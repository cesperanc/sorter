/**
* @file webslib.h
* @brief header file for the web server functions
* @date 2009/12/21 File creation
*
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/



#ifndef WEBSLIB_H_
#define WEBSLIB_H_

/**
 * @brief To store the common data of the web server thread
 */
typedef struct{
	int shutdown;					/**< @brief flag to enable a socket exit */
	int sockfd;						/**< @brief reference to a socket */
	char *port;						/**< @brief reference to port number string */
	char *content;					/**< @brief reference to the shared content string */
	pthread_mutex_t *mutex;			/**< @brief reference to the mutex for content access */
} WEB_SERVER_PARAMS_T;

void sigchld_handler(int);
void *get_in_addr(void*);
void create_web_server(char *, int, pthread_t *, WEB_SERVER_PARAMS_T *);
void *web_serve(void *);
void shutdown_web_server(pthread_t *, WEB_SERVER_PARAMS_T *);
char* update_content(WEB_SERVER_PARAMS_T *, char *, char*, char*, char*);
char* merge_strings(char*, char*);

#endif /* WEBSLIB_H_ */
