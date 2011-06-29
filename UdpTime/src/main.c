/**
* \mainpage
* The UdpTime is an application serve the time using UDP
* 
* 
* @file main.c
* @brief Main source file for the UdpTime program
* @date 2009-12-10 File creation
* @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "3rd/debug.h"

#include "includes/definitions.h"
#include "includes/aux.h"
#include "main.h"

/**
 * @brief The main program function
 * @param argc integer with the number of command line options
 * @param argv *char[] with the command line options
 * @return integer 0 on a successfully exit, another integer value otherwise
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int main(int argc, char *argv[]){
	long port=1234;							// to store the port number to serve the requests
	char *endptr;							// to store the invalid characters from the conversion of the parameter given
	int sock_fd;							// to store the an application socket
	struct sockaddr_in ser_addr;			// to store the sockaddr_in structure

	system("clear");
	if(argc!=2){
		MY_DEBUG("\nINVALID_PARAMETERS\n");
		printf("The arguments specified are not valid.\nUse: %s <port to listen>\n",argv[0]);
		exit(M_INVALID_PARAMETERS);
	}

	errno = 0;
	// Convert the given parameter to a long value
	port = strtol(argv[1], &endptr, 0);
	/* Verify for errors */
	if ((errno == ERANGE && (port >= LONG_MAX || port <= LONG_MIN)) || (errno != 0 && port == 0)) {
		MY_DEBUG("\nNUMBER_CONVERSION_ERROR\n");
		printf("Unable to convert the parameter '%s' to a valid port number.\nUse: %s <port to listen>\n", argv[1], argv[0]);
		exit(M_NUMBER_CONVERSION_ERROR);
	}
	if (endptr == argv[1]) {
		MY_DEBUG("\nNO_DIGITS_FOUND\n");
		printf("No digits found in the parameter '%s'.\nUse: %s <port to listen>\n", argv[1], argv[0]);
		exit(M_NO_DIGITS_FOUND);
	}
	if(port<PORT_RANGE_MIN || port>PORT_RANGE_MAX){
		MY_DEBUG("\nPORT_OUT_OF_RANGE\n");
		printf("The port %ld is out of the allowed range port numbers. \nUse: %s <port to listen>, were <port to listen> is a number between %d and %d\n", port, argv[0], PORT_RANGE_MIN, PORT_RANGE_MAX);
		exit(M_PORT_OUT_OF_RANGE);
	}


	/* Creates the socket */
	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		ERROR(M_SOCKET_CREATION_ERROR, "\nError while creating the application socket.\n");
	}

	/* Initializes the sockaddr_in structure with the socket information */
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ser_addr.sin_port = htons(port);

	/* Register the socket */
	if (bind(sock_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) == -1){
		ERROR(M_BIND_SOCKET_ERROR, "\nError binding the socket.\n");
	}

	printf("%s ready and listening at port %ld\n", argv[0], port);
	while (1) {
		/* Wait for an request */
		wait_for_request(sock_fd);
	}
	//close(sock_fd);
		
	return 0;
}

/**
 * @brief Wait for a client request and send back a time of day string
 * @param sock_fd integer with the server socket reference
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void wait_for_request(int sock_fd){
	struct sockaddr_in cli_addr;		// To store the socket of the client
	socklen_t cli_length;				// To store the size of the socket structure of the client
	char ip[20];						// To store the IP address of the client
	struct timeval timestamp;			// To store the time value
	char response[255];					// To store the response to send to the client


    cli_length=sizeof(cli_addr);
    // Receive the request from the client
    if (recvfrom(sock_fd, NULL, 0, 0, (struct sockaddr *) &cli_addr, &cli_length) == -1){
    	ERROR(M_RECVFROM_ERROR, "\nError occurred while receiving the request from the client.\n");
    }
    // Get the time of the day
    gettimeofday(&timestamp, NULL);
    // Store the time of the day in the response
	sprintf(response, "%llu", timestamp.tv_sec*1000000ULL+timestamp.tv_usec);
	// Convert the client address to a valid string
	inet_ntop (AF_INET, &cli_addr.sin_addr, ip, sizeof (ip));
    printf("Client %s@%d request. Sending: %s.\n", ip , htons (cli_addr.sin_port), response);

    if (sendto(sock_fd, response, strlen(response), 0, (struct sockaddr *) &cli_addr, cli_length) < 0){
    	ERROR(M_SENDTO_ERROR, "\nError while sending the response to the client\n");
    }
}
