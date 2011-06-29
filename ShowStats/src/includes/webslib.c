/**
* @file webslib.c
* @brief source file for the web server functions
* @date 2009/12/21 File creation
* @see http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#simpleserver for references
*
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#include "../3rd/debug.h"
#include "definitions.h"
#include "aux.h"
#include "webslib.h"

/**
 * @brief Handler to prevent the zombie state of the children processes
 * @param s signal received
 *
 * @author Brian "Beej Jorgensen" Hall <beej@beej.us>
 */
void sigchld_handler(int s){
	switch(s){
		default:
			while(waitpid(-1, NULL, WNOHANG) > 0);
	}

}

/**
 * @brief Get the sockaddr in the IPv4 or IPv6 format
 * @param sa with the sockaddr to cast
 *
 * @author Brian "Beej Jorgensen" Hall <beej@beej.us>
 */
void *get_in_addr(void* sa){
	if (((struct sockaddr*) sa)->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * @brief Create a web server to listen on the specified port with maximum simultaneous connections
 * @param port string with the port number
 * @param connections integer with the maximum number of simultaneous connections to server
 * @param t_web_server with the web server thread to use
 * @param web_server_params structure to store the thread params
 *
 * @author Brian "Beej Jorgensen" Hall <beej@beej.us>, Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void create_web_server(char* port, int connections, pthread_t *t_web_server, WEB_SERVER_PARAMS_T *web_server_params){
	int rv;											// To store the return value of getaddrinfo (for error handling)
	int sockfd;  									// Listen on sock_fd, New connections on new_fd
	int yes=TRUE;									// To flag the reuse of a socket
	struct addrinfo hints;							// To store relevant information about our service
	struct addrinfo *servinfo, *p;					// To store the results of getaddrinfo queries

	// Fill hints with nothing, to make sure the structure is empty
	memset(&hints, 0, sizeof(hints));
	// We don't want to specify the IP version yet
	hints.ai_family = AF_UNSPEC;
	// We want a TCP socket
	hints.ai_socktype = SOCK_STREAM;
	// Use the local address
	hints.ai_flags = AI_PASSIVE;

	// Query information about the port and the network
	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		ERROR(M_GETADDRINFO_ERROR, "\nFailed to get information about the port %d (%s)\n", port, gai_strerror(rv));
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		// Tries to create the socket
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			MY_DEBUG("Server socket error\n");
			continue;
		}

		// Allows the reuse of the port unless another service is already using it (don't block the port on exit)
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			ERROR(M_SETSOCKOPT_ERROR, "\nError while setting the socket options\n");
		}

		// Bind to the socket
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			MY_DEBUG("Server bind error\n");
			continue;
		}

		break;
	}
	// If we don't have a successfully binding
	if (p == NULL)  {
		ERROR(M_BIND_SERVER_ERROR, "\nThe web server failed to bind\n");
	}

	// Free the resource
	freeaddrinfo(servinfo);

	// Start listening on the socket
	if (listen(sockfd, connections) == -1) {
		ERROR(M_SOCKET_LISTEN_ERROR, "\nThe web server failed to listen\n");
	}

	web_server_params->port = port;
	web_server_params->sockfd = sockfd;
	web_server_params->shutdown = FALSE;
	// Create the web server thread
	if (pthread_create(t_web_server, NULL, web_serve, web_server_params) != 0) {
		ERROR(M_PTHREAD_CREATE_FAILED, "\nWeb Server thread creation failed.\n");
	}
}

/**
 * @brief Serve the content using the given args
 * @param arg WEB_SERVER_PARAMS_T structure with the thread information
 *
 * @author Brian "Beej Jorgensen" Hall <beej@beej.us>, Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void *web_serve(void *arg){
	WEB_SERVER_PARAMS_T *web_server_params = (WEB_SERVER_PARAMS_T *) arg;
	int new_fd;  													// New connections on new_fd
	struct sockaddr_storage their_addr; 							// Connector's address information
	socklen_t sin_size;												// Connector's address information size
	char client_address[INET6_ADDRSTRLEN];							// To store a client address
	char* response = "HTTP/1.0 200 OK\r\n";							// Default HTTP 1.0 response
	char headers[MAXCHARS];											// To store some HTTP headers
	char* content=NULL;

	MY_DEBUG("Waiting for connections on port %s...\n", web_server_params->port);
	sin_size = sizeof(their_addr);
	while(1) {  // main accept() loop
		// Lock waiting to accept a connection, and if it fails, loop to accept another one
		if((new_fd = accept(web_server_params->sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
			if(web_server_params->shutdown==TRUE){
				MY_DEBUG("Got the shutdown signal\n");
				break;
			}
			MY_DEBUG("Connection accept failed\n");
			continue;
		}

		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), client_address, sizeof(client_address));
		MY_DEBUG("Got a connection from %s\n", client_address);

		// Try to lock the mutex for read
		if (pthread_mutex_lock(web_server_params->mutex) != 0) {
			MY_DEBUG("\nWeb Server thread lock failed\n");
		}else{
			// If the mutex lock was successful, build some nice HTML content
			content = update_content(NULL, NULL, "<html><head><title>Show Stats</title></head><body><table border='1'><caption>Show Stats Output:</caption><tr><td>filename</td><td>nlines</td><td>algorithm</td><td>niterations</td><td>nswaps</td><td>time</td></tr>", web_server_params->content, "</table></body></html>");
			// Update the headers with the content length
			sprintf(headers, "Content-Type: text/html\r\nContent-Length:%d\r\n\r\n", (int) strlen(content));

			if(write(new_fd, response, strlen(response))==-1){
				MY_DEBUG("\nFailed to write the HTTP response\n");
			}else if(write(new_fd, headers, strlen(headers))==-1){
				MY_DEBUG("\nFailed to write the HTTP headers\n");
			}else if(write(new_fd, content, strlen(content))==-1){
				MY_DEBUG("\nFailed to write the HTML content\n");
			}
			// Free the used memory for content
			if(content!=NULL){
				free(content);
			}

			// Unlock the mutex
			if (pthread_mutex_unlock(web_server_params->mutex) != 0) {
				MY_DEBUG("\nWeb Server thread unlock failed\n");
			}
		}
		// The parent doesn't need to send
		close(new_fd);
	}
	return NULL;
}

/**
 * @brief Shutdown the web server thread
 * @param t_web_server with the web server thread to shutdown
 * @param web_server_params structure to recover the thread params
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void shutdown_web_server(pthread_t *t_web_server, WEB_SERVER_PARAMS_T *web_server_params){
	web_server_params->content = update_content(web_server_params, web_server_params->content,"<h1>", "The server is shutting down...", "</h1>");
	MY_DEBUG("Shutting down the Web Server\n");
	// Mark the server for shutdown
	web_server_params->shutdown=TRUE;
	// Shutdown the socket in both ways
	shutdown(web_server_params->sockfd, SHUT_RDWR);
	// Close the listening socket
	close(web_server_params->sockfd);
	// Wait for the thread to exit
	pthread_join(*t_web_server, NULL);
	// Destroy the mutex
	if (pthread_mutex_destroy(web_server_params->mutex) != 0) {
		MY_DEBUG("\npthread_mutex_destroy() failed!\n");
	}
	if(web_server_params->content!=NULL){
		free(web_server_params->content);
	}
	MY_DEBUG("Web Server stopped\n");

}

/**
 * @brief Build a string with a beginning, a middle and an end
 * @param web_server_params structure to use on the thread lock
 * @param content with the reference to be free'd and appointed to the new string reference
 * @param begin string with the first part of the string
 * @param middle string to put in the middle
 * @param end string to add at the end of the generated string
 * @return a memory reference for the new string
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
char* update_content(WEB_SERVER_PARAMS_T *web_server_params, char* content, char* begin, char* middle, char* end){
	char* aux=NULL, *aux_tmp=NULL;
	if (web_server_params!=NULL && pthread_mutex_lock(web_server_params->mutex) != 0) {
		MY_DEBUG("\nWeb Server thread lock failed\n");
	}else{
		aux = merge_strings(aux, begin);
		aux_tmp = aux;
		aux = merge_strings(aux, middle);
		if(aux_tmp!=NULL){
			free(aux_tmp);
		}
		aux_tmp = aux;
		aux = merge_strings(aux, end);
		if(aux_tmp!=NULL){
			free(aux_tmp);
		}
		if(content!=NULL){
			free(content);
			content = aux;
		}
		if (web_server_params!=NULL && pthread_mutex_unlock(web_server_params->mutex) != 0) {
			MY_DEBUG("\nWeb Server thread unlock failed\n");
		}
	}
	return aux;
}

/**
 * @brief Merge to strings
 * @param left string to put on the left
 * @param right string to put on the right
 * @return a memory reference for the new string
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
char* merge_strings(char* left, char* right){
	unsigned int left_length=0;
	unsigned int right_length=0;
	char *aux=NULL;

	if(left!=NULL){
		left_length = strlen(left);
	}else{
		left_length=0;
	}

	if(right!=NULL){
		right_length = strlen(right);
	}else{
		right_length=0;
	}

	aux = (char *)calloc(sizeof(char)*(left_length+right_length+1), sizeof(char));
	if(left!=NULL){
		strcpy(aux, left);
	}else{
		strcpy(aux, "");
	}
	if(right!=NULL){
		strcat(aux, right);
	}

	return aux;
}
