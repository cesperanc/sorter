/**
* @file sorterlib.c
* @brief source file for the sorter functions
* @date 2009/10/31 File creation
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../3rd/debug.h"
#include "../3rd/semaforos.h"
#include "definitions.h"
#include "aux.h"
#include "commonlib.h"
#include "sorterlib.h"

/**
 * @brief This function allocates the necessary memory to reference the lines of a file in memory
 * @param numlines integer with the number of lines to allocate
 * @return FILE_LINES_T pointer with the base lines structure
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
FILE_LINES_T* allocate_memory_for_lines(int numlines){
	FILE_LINES_T* flines = NULL;
	void** lines = NULL;
	int a;

	// Allocate the memory for the structure
	if((flines=(FILE_LINES_T *)malloc(sizeof(FILE_LINES_T)))!=NULL){
		// Allocate the memory to store the reference to all the lines
		if((lines=malloc(sizeof(int*)*numlines))!=NULL){
			// Reset the reference to each line
			for (a=0;a<numlines;a++){
				lines[a] = NULL;
			}
			// Store the reference in the structure
			flines->lines=lines;
			// Store the number of lines on the structure
			flines->num_lines = numlines;
		}else{
			ERROR(M_FAILED_MEMORY_ALLOCATION,"Error in memory allocation");
		}
	}else{
		ERROR(M_FAILED_MEMORY_ALLOCATION,"Error in memory allocation");
	}
	return flines;
}

/**
 * @brief Free the memory referenced by the given parameter
 * @param flines FILE_LINES_T with the memory references
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void free_memory_of_lines(FILE_LINES_T* flines){
	int a;

	for(a=flines->num_lines-1;a>=0;a--){
		free(flines->lines[a]);
		flines->lines[a]=NULL;
	}
	free(flines->lines);
	flines->lines=NULL;
	flines->num_lines=0;
	free(flines);
	flines=NULL;
}

/**
 * @brief Create a partial clone from a FILE_LINES_T object (just the index)
 * @param flines FILE_LINES_T to clone
 * @return FILE_LINES_T clone
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
FILE_LINES_T* clone_of_lines(FILE_LINES_T* flines){
	FILE_LINES_T *flines_clone = NULL;
	int a=0;

	if((flines_clone = allocate_memory_for_lines(flines->num_lines))!=NULL){
		flines_clone->num_lines = flines->num_lines;
		for(a=0;a<flines->num_lines;a++){
			flines_clone->lines[a]=flines->lines[a];
		}
	}
	return flines_clone;
}

/**
 * @brief Copy the lines references of a FILE_LINES_T object to another with the same number of lines
 * @param flines_src FILE_LINES_T source
 * @param flines_dest FILE_LINES_T destination
 * @return integer TRUE on success FALSE on error
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int copy_lines_from_clone(FILE_LINES_T* flines_src, FILE_LINES_T* flines_dest){
	int a=0;

	if(flines_src->num_lines!=flines_dest->num_lines){
		return FALSE;
	}

	for(a=0;a<flines_src->num_lines;a++){
		flines_dest->lines[a]=flines_src->lines[a];
	}
	return TRUE;
}

/**
 * @brief Free the allocated memory for a FILE_LINES_T clone
 * @param flines FILE_LINES_T to free
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void free_memory_of_clone_of_lines(FILE_LINES_T* flines){
	int a;

	for(a=flines->num_lines-1;a>=0;a--){
		flines->lines[a]=NULL;
	}
	free(flines->lines);
	flines->lines=NULL;
	flines->num_lines=0;
	free(flines);
	flines=NULL;
}

/**
 * @brief This function allocates the necessary memory to reference the statistical information of a sort operation
 * @param filename with the filename to use as reference
 * @param algorithm with the algorithm name used on the operation
 * @return ALGORITHM_STAT_T pointer with the structure to store the data
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
ALGORITHM_STAT_T* allocate_memory_for_stat(char* filename, char* algorithm){
	ALGORITHM_STAT_T* stat = NULL;

	// Allocate the memory for the structure
	if((stat=(ALGORITHM_STAT_T *)malloc(sizeof(ALGORITHM_STAT_T)))!=NULL){
		stat->filename = filename;
		stat->algorithm = algorithm;
		stat->niterations = 0;
		stat->nlines = 0;
		stat->nswaps = 0;
		stat->time=0;
	}else{
		ERROR(M_FAILED_MEMORY_ALLOCATION,"Error in memory allocation");
	}
	return stat;
}

/**
 * @brief Reset the data structure for the stat object
 * @param stat ALGORITHM_STAT_T to reset
 * @param filename with the filename to use as reference
 * @param algorithm with the algorithm name used on the operation
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void reset_stat(ALGORITHM_STAT_T* stat, char* filename, char* algorithm){
	stat->filename = filename;
	stat->algorithm = algorithm;
	stat->niterations = 0;
	stat->nlines = 0;
	stat->nswaps = 0;
	stat->time=0;
}

/**
 * @brief Free the memory referenced by the given parameter
 * @param stat ALGORITHM_STAT_T to free
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void free_memory_of_stat(ALGORITHM_STAT_T* stat){
	reset_stat(stat, NULL, NULL);
	free(stat);
	stat=NULL;
}

/**
 * @brief Initializes a statistical control controller and creates the shared statistical control memory segment
 * @param controller_stat with the reference to the statistical control controller to initialize
 * @param path_name path name for the System V IPC key
 * @param number_of_files to store information about in the shared memory
 * @param number_of_algorithms to store information about in the shared memory
 * @return integer 0 if the controller was successfully initialized, the error code otherwise
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int initializes_controller_stat(CONTROLLER_STAT_T* controller_stat,  char* path_name, int number_of_files, int number_of_algorithms){
	key_t control_key;													/**< @brief store a System V IPC key for the control */
	key_t data_key;														/**< @brief store a System V IPC key for the data */

	// Generate a shared memory key for the control key
	if((control_key = ftok(path_name, 'c'))==-1){
		MY_DEBUG("\nShared memory key generation for the control data failed.\n");
		return M_FTOK_FAILED_FOR_CONTROL_KEY;
	}
	// Creates a shared memory zone for the control data
	if((controller_stat->shm_control_id = shmget(control_key, sizeof(CONTROL_STAT_T), 0600|IPC_CREAT|IPC_EXCL))==-1){
		MY_DEBUG("\nError while creating the shared memory for the control data with the key [%x]. Is another %s running? If not, remove the shared memory segment(s)\n", control_key, path_name);
		return M_SHMGET_FAILED_FOR_CONTROL_ID;
	}
	// Creates the semaphore for the shared memory control access
	if((controller_stat->sem_control_id = sem_create(control_key, 2, 0600|IPC_CREAT|IPC_EXCL))==-1){
		MY_DEBUG("\nError while creating the semaphore for the access to the shared memory control data with the key [%x], Is the %s running? If not, remove the semaphores", control_key, path_name);
		return M_SEMCREATE_FAILED_FOR_CONTROL;
	}
	// Initializes the semaphore for the access to the controller
	if(sem_setvalue(controller_stat->sem_control_id, MUTEX_CONTROL_STATS, 0)==-1){
		MY_DEBUG("\nError while setting the value for the semaphore\n");
		return M_SEMSET_FAILED_FOR_CONTROL;
	}
	// Initializes the semaphore for controlling the exit
	if(sem_setvalue(controller_stat->sem_control_id, MUTEX_EXIT, 1)==-1){
		MY_DEBUG("\nError while setting the value for the semaphore\n");
		return M_SEMSET_FAILED_FOR_CONTROL;
	}
	// Attach shared memory for the control to this process
	if((controller_stat->control_data = shmat(controller_stat->shm_control_id, NULL, 0))==(void *)-1){
		MY_DEBUG("\nError while attaching on the shared memory segment for the control data\n");
		return M_SHMAT_FAILED_FOR_CONTROL;
	}


	// Generate a shared memory key for the data key
	if((data_key = ftok(path_name, 'd'))==-1){
		MY_DEBUG("\nShared memory key generation for the data failed.\n");
		return M_FTOK_FAILED_FOR_DATA;
	}
	// Creates a shared memory zone for the data
	if((controller_stat->shm_data_id = shmget(data_key, sizeof(SHARED_ALGORITHM_STAT_T)*number_of_files*number_of_algorithms, 0600|IPC_CREAT|IPC_EXCL))==-1){
		MY_DEBUG("\nError while creating the shared memory for the data with the key [%x]. Is another %s running? If not, remove the shared memory segment(s)\n", data_key, path_name);
		return M_SHMGET_FAILED_FOR_DATA;
	}
	// Creates the semaphore for the shared memory data access
	if((controller_stat->sem_data_id = sem_create(data_key, 2, 0600|IPC_CREAT|IPC_EXCL))==-1){
		MY_DEBUG("\nError while creating the semaphore for the access to the shared memory data with the key [%x]. Is another %s running? If not, remove the shared memory segment(s)\n", data_key, path_name);
		return M_SEMCREATE_FAILED_FOR_DATA;
	}
	// Attach shared memory for the data to this process
	if((controller_stat->stats = (SHARED_ALGORITHM_STAT_T*) shmat(controller_stat->shm_data_id, NULL, 0))==(void *)-1){
		MY_DEBUG("\nError while attaching on the shared memory segment for the data\n");
		return M_SHMAT_FAILED_FOR_DATA_STATS;
	}
	// Initializes the semaphore for the access to the data
	if(sem_setvalue(controller_stat->sem_data_id, MUTEX_DATA_ACCESS, 1)==-1){
		MY_DEBUG("\nError while setting the value for the semaphore\n");
		return M_SEMSET_FAILED_FOR_DATA;
	}
	// Initializes the semaphore for the data release
	if(sem_setvalue(controller_stat->sem_data_id, MUTEX_DATA_AVAILABLE, 0)==-1){
		MY_DEBUG("\nError while setting the value for the semaphore\n");
		return M_SEMSET_FAILED_FOR_DATA;
	}


	controller_stat->control_data->to_exit = FALSE;
	controller_stat->control_data->locked = FALSE;
	controller_stat->control_data->index_stat = 0;
	controller_stat->control_data->number_of_files = number_of_files;
	controller_stat->control_data->number_of_algorithms = number_of_algorithms;

	return 0;
}

/**
 * @brief detach the statistical control from the shared memory and try to release that segment
 * @param controller_stat with the reference to the statistical control controller to lock access
 * @return integer 0 if the controller was successfully initialized, the error code otherwise
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int remove_controller_stat(CONTROLLER_STAT_T* controller_stat){
	lock_data_for_access(controller_stat);
	lock_controller_stat(controller_stat);

	// Detach shared memory for the data
	if (shmdt(controller_stat->stats) == -1){
		ERROR(M_SHMDT_FAILED, "\nStats\n");
		return M_SHMDT_FAILED;
	}
	//controller_stat->control_data->stats = NULL;

	// Detach shared memory for the control data
	if (shmdt(controller_stat->control_data) == -1){
		ERROR(M_SHMDT_FAILED, "\nControl\n");
		return M_SHMDT_FAILED;
	}
	//controller_stat->control_data = NULL;

	// Remove the shared memory segment for the data
	if (shmctl(controller_stat->shm_data_id, IPC_RMID, 0) == -1){
		return M_SHMCTL_FAILED;
	}

	// Remove the shared memory segment for the control
	if (shmctl(controller_stat->shm_control_id, IPC_RMID, 0) == -1){
		return M_SHMCTL_FAILED;
	}

	// Remove the semaphore for the data
	if (sem_delete(controller_stat->sem_data_id) == -1){
		return M_SEMDELETE_FAILED;
	}

	// Remove the semaphore for the control
	if (sem_delete(controller_stat->sem_control_id) == -1){
		return M_SEMDELETE_FAILED;
	}

	return 0;
}

/**
 * @brief detach the statistical control from the shared memory and try to release that segment
 * @param controller_stat with the reference to the statistical control controller
 * @param stat_data to append to the structure
 * @param toExit with a boolean operation to flag the exit request of the application
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void append_stat(CONTROLLER_STAT_T* controller_stat, ALGORITHM_STAT_T* stat_data, int toExit){
	// Lock the data for access
	lock_data_for_access(controller_stat);
	// Lock the controller for statistics
	lock_controller_stat(controller_stat);

	// Copy the values
	copy_stat(&(controller_stat->stats[controller_stat->control_data->index_stat]), *stat_data);
	// If is to exit the application, signal it
	if(toExit){
		controller_stat->control_data->to_exit=TRUE;
	}
	// Forward the index
	controller_stat->control_data->index_stat++;

	// Release the controller
	release_controller_stat(controller_stat);
	// If the that wasn't released yet, release it
	release_new_data(controller_stat);
	// Release the data for access
	release_data_for_access(controller_stat);
}

/**
 * Copy an ALGORITHM_STAT_T to an SHARED_ALGORITHM_STAT_T
 * @param stat_dest the reference to the SHARED_ALGORITHM_STAT_T to store the copy
 * @param stat_src the ALGORITHM_STAT_T to be copied
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void copy_stat(SHARED_ALGORITHM_STAT_T* stat_dest, ALGORITHM_STAT_T stat_src){
	// Copy the algorithm until the max size of the buffer
	snprintf(stat_dest->algorithm, sizeof(char)*(MAXCHARS-1), "%s", stat_src.algorithm);
	stat_dest->algorithm[MAXCHARS]='\0'; // terminate the string

	// Copy the filename until the max size of the buffer
	snprintf(stat_dest->filename, sizeof(char)*(MAXCHARS-1), "%s", stat_src.filename);
	stat_dest->filename[MAXCHARS]='\0'; // terminate the string

	// Copy the others values
	stat_dest->niterations = stat_src.niterations;
	stat_dest->nlines = stat_src.nlines;
	stat_dest->nswaps = stat_src.nswaps;
	stat_dest->time = stat_src.time;
}

/**
 * @brief lock the exit if a showStats is still listening
 * @param controller_stat with the reference to the statistical control controller to lock exit
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void wait_for_exit_unlock(CONTROLLER_STAT_T* controller_stat){
	if(sem_down(controller_stat->sem_control_id, MUTEX_EXIT)==-1){
		ERROR(M_SEMDOWN_FAILED, "\nError while locking the exit");
	}
}

/**
 * @brief Read the file contents, line by line
 * @param filename char* with the filename to load from
 * @param maxchars integer with the maximum number of characters to read
 * @return FILE_LINES_T pointer with the base lines structure
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
FILE_LINES_T* read_file(char* filename, int maxchars){
	FILE* file = NULL;
	FILE_LINES_T *flines = NULL;
	char line[maxchars+1];
	int numlines = 0, a=0;

	// Test if the line count was successful
	if((numlines = count_file_lines(filename,maxchars))<0){
		ERROR(M_FAILED_RETRIEVE_FILE_LINES_COUNT, "Failed to count the number of lines from file %s\n", filename);
	}

	// Reserve the memory for the data structure
	if((flines = allocate_memory_for_lines(numlines))!=NULL){
		// Open the file
		file = fopen(filename,"r");
		if(file!= NULL){
			a=0;
			// Load the file, line by line, each line with a maximum size of maxchars characters
			while(fgets(line, maxchars, file) != NULL ){
				// Verify if we don't have more lines on the file than the reserved memory (this could happen if the file was updated during the loading)
				if(a>numlines){
					ERROR(M_FILE_BIGGER_THAN_EXPECTED, "The file has more lines then the previous count.");
				}
				// Allocate the necessary memory for the line
				if((flines->lines[a] = malloc(sizeof(char)*(strlen(line)+1)))!=NULL){
					// Store the reference for the line
					strcpy((char*)flines->lines[a], line);
				}else{
					ERROR(M_FAILED_MEMORY_ALLOCATION, "Error in memory allocation");
				}
				// Proceeds to the next free reference
				a++;
			}
			// Close the file handler
			fclose (file);
		}else{
			ERROR(M_FAILED_FILE_READ, "\nError reading the file %s",filename);
		}
	}
	return flines;
}

/**
 * Sort the lines with the specified algorithm function
 *
 * @param flines to be sorted
 * @param algorithm_function to be used as the sort operator
 * @param stat ALGORITHM_STAT_T with the structure to store the statistical data of the sort operation
 * @param rur_info with the structure with the information about the remote UDO request to use
 * @return FILE_LINES_T sorted
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
FILE_LINES_T* sort_lines(FILE_LINES_T* flines, ALGORITHM_FUNC algorithm_function, ALGORITHM_STAT_T* stat, REMOTE_UDP_REQUEST_T rur_info){
	struct timeval start, end;
	unsigned long long udp_start=0, udp_end=0;

	if(rur_info.sock_fd>0){
		udp_start = get_udp_time(rur_info);
		// Call the sorter algorithm function
		algorithm_function(flines, stat);
		udp_end = get_udp_time(rur_info);

		// Save the difference
		stat->time = ((float)(udp_end-udp_start))/1000;
	}else{
		// start ticking
		gettimeofday(&start, NULL);
		// Call the sorter algorithm function
		algorithm_function(flines, stat);
		// stop timer
		gettimeofday(&end, NULL);

		// Save the difference
		stat->time = time_diff(start,end)*1000;
	}

	return flines;
}

/**
 * @brief Get a timestamp using the given parameters
 * @param rur_info with the connection information
 * @return timestamp returned by the server
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
unsigned long long get_udp_time(REMOTE_UDP_REQUEST_T rur_info){
	socklen_t server_length;
	char response[255];
	int endindex;
	unsigned long long timestamp;

	server_length=sizeof(*(rur_info.server_addr));
	// Send a request to the server
	if (sendto(rur_info.sock_fd, NULL, 0, 0, (struct sockaddr *) rur_info.server_addr, server_length) < 0){
		ERROR(M_SENDTO_ERROR, "\nError while sending the request to the server\n");
	}
	// Get a response from the server
	if ((endindex = recvfrom(rur_info.sock_fd, response, sizeof(char)*254, 0, (struct sockaddr *) rur_info.server_addr, &server_length)) < 0){
		ERROR(M_RECVFROM_ERROR, "\nError while receiving the response from the server\n");
	}
	// Terminate the string from the response
	response[endindex]=0;
	// Convert the string to a timestamp
	sscanf(response,"%llu",&timestamp);

	return timestamp;
}

/**
 * @brief Initializes a UDP socket connection (validating the given IP address)
 * @param rur_info with the structure to store the data
 * @param ip_address the destination IP address
 * @param port the destination port
 * @return 0 on success, the error code otherwise
 */
int initialize_udp_connection(REMOTE_UDP_REQUEST_T* rur_info, char* ip_address, int port){
	/* Creates the socket */
	if ((rur_info->sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		ERROR(M_SOCKET_CREATION_ERROR, "Error while creating the socket.");
	}

	/* Fill the server address with 0's */
	memset(rur_info->server_addr, 0, sizeof(*(rur_info->server_addr)));
	rur_info->server_addr->sin_family = AF_INET;
	rur_info->server_addr->sin_port = htons(port);

	/* Parse and fill the server address */
	switch(inet_pton (AF_INET, ip_address, &(rur_info->server_addr->sin_addr))) {
	  case 0:
		  close(rur_info->sock_fd);
		  printf("The IP address '%s' is not valid\n", ip_address);
		  return M_INVALID_IP_ADDRESS;
	  case -1:
		  close(rur_info->sock_fd);
		  printf ("Unknown IP address: %s\n", ip_address);
		  return M_UNKNOWN_IP_ADDRESS;
	}
	// Verify the port range
	if(port<PORT_RANGE_MIN || port>PORT_RANGE_MAX){
		close(rur_info->sock_fd);
		printf("The port %d is out of the allowed range port numbers ([%d,%d])\n", port, PORT_RANGE_MIN, PORT_RANGE_MAX);
		return M_PORT_OUT_OF_RANGE;
	}
	return 0;
}

/**
 * @brief Send a result to a remove UDP results server
 * @param rur_result with the connection information
 * @param stat with the statistical data
 * @param nickname with the name of the group
 * @param machine_model_name with the model name of the computer which sorted the file
 * @param md5sum of the sorted file
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void send_udp_result(REMOTE_UDP_REQUEST_T rur_result, ALGORITHM_STAT_T* stat, char* nickname, char* machine_model_name, char* md5sum){
	socklen_t server_length;
	char data[MAXCHARS]="";					// To store the data to send to the server
	char response[MAXCHARS]="";				// To store the server response
	int endindex;							// To store the size of the response

	if(rur_result.sock_fd>0){
		// Build the data string to send
		sprintf(data, "%s,%s,%s,%s,%s,%.0f", nickname, machine_model_name, stat->filename, md5sum, stat->algorithm, stat->time);

		server_length=sizeof(*(rur_result.server_addr));
		// Send a request to the server
		if (sendto(rur_result.sock_fd, data, strlen(data)*sizeof(char), 0, (struct sockaddr *) rur_result.server_addr, server_length) < 0){
			ERROR(M_SENDTO_ERROR, "\nError while sending the data to the server\n");
		}

		// Get a response from the server
		if ((endindex = recvfrom(rur_result.sock_fd, response, sizeof(char)*MAXCHARS, 0, (struct sockaddr *) rur_result.server_addr, &server_length)) < 0){
			ERROR(M_RECVFROM_ERROR, "\nError while receiving the response from the server\n");
		}
		// Terminate the string from the response
		response[endindex]=0;
		MY_DEBUG("\nThe server response was %s\n",response);
	}
}

/**
 * Save the lines to the file
 *
 * @param filename to write to
 * @param flines with the lines to store on the file
 * @return integer TRUE on success, FALSE on error
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int save_file(char* filename, FILE_LINES_T* flines){
	FILE* file = NULL;
	int a=0;

	if((file=fopen(filename,"w"))!=NULL){
		for(a=0; a<flines->num_lines; a++){
			fprintf(file,"%s",(char *) flines->lines[a]);
		}

		fclose(file);
		return TRUE;
	}
	return FALSE;
}
