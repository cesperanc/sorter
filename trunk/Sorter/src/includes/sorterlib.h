/**
 * @file sorterlib.h
 * @brief Header file for the sorter functions
 * @date 2009/10/31 File creation
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */



#ifndef SORTERLIB_H_
#define SORTERLIB_H_

/*
 * data structures
 */
/**
 * @brief Type declaration to a structure to store the references to lines of files, and the number of lines that the structure has access
 *
 * @see allocate_memory_for_lines for reference
 */
typedef struct file_lines {
	void** lines; 	/**< @brief reference to the lines. */
	int num_lines;	/**< @brief reference to the number of lines. */
} FILE_LINES_T;

/**
 * @brief Type declaration that represents a pointer to sort function
 *
 * @param FILE_LINES_T with the lines to sort
 * @return FILE_LINES_T the the sorted lines
 */
typedef FILE_LINES_T* (*ALGORITHM_FUNC) (FILE_LINES_T*, ALGORITHM_STAT_T*);


/**
 * @brief Type declaration to a structure to store the references to a remote UDP request to a server
 */
typedef struct remote_udp_request {
	int sock_fd; 						/**< @brief reference to the socket to use in the communication process */
	struct sockaddr_in *server_addr;	/**< @brief reference to the server address socket to use */
} REMOTE_UDP_REQUEST_T;

/*
 * functions prototypes
 */
FILE_LINES_T *allocate_memory_for_lines(int);
void free_memory_of_lines(FILE_LINES_T*);
FILE_LINES_T* clone_of_lines(FILE_LINES_T*);
int copy_lines_from_clone(FILE_LINES_T*, FILE_LINES_T*);
void free_memory_of_clone_of_lines(FILE_LINES_T*);
ALGORITHM_STAT_T* allocate_memory_for_stat(char*, char*);
void reset_stat(ALGORITHM_STAT_T*, char*, char*);
void free_memory_of_stat(ALGORITHM_STAT_T*);
int initializes_controller_stat(CONTROLLER_STAT_T* , char*, int , int);
int remove_controller_stat(CONTROLLER_STAT_T*);
void append_stat(CONTROLLER_STAT_T*, ALGORITHM_STAT_T*, int);
void copy_stat(SHARED_ALGORITHM_STAT_T*, ALGORITHM_STAT_T);
void wait_for_exit_unlock(CONTROLLER_STAT_T*);
FILE_LINES_T *read_file(char*, int);
FILE_LINES_T* sort_lines(FILE_LINES_T*, ALGORITHM_FUNC, ALGORITHM_STAT_T*, REMOTE_UDP_REQUEST_T);
unsigned long long get_udp_time(REMOTE_UDP_REQUEST_T);
int initialize_udp_connection(REMOTE_UDP_REQUEST_T*, char*, int);
void send_udp_result(REMOTE_UDP_REQUEST_T, ALGORITHM_STAT_T*, char*, char*, char*);
int save_file(char*, FILE_LINES_T*);

#endif /* SORTERLIB_H_ */
