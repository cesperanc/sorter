/**
* \mainpage
* The Sorter is an application to sort data files using various algorithms
* 
* 
* @file main.c
* @brief Main source file for the Sorter program
* @date 2009-10-28 File creation
* @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <arpa/inet.h>

#include "3rd/debug.h"
#include "3rd/sorter_options.h"
#include "3rd/semaforos.h"

#include "includes/definitions.h"
#include "includes/aux.h"
#include "includes/commonlib.h"
#include "includes/sorterlib.h"
#include "includes/sorters.h"
#include "main.h"

/**
 * @brief Flag for the no sigint signal
 */
char* _sigint_time=NULL;

/**
 * @brief The main program function
 * @param argc integer with the number of command line options
 * @param argv *char[] with the command line options
 * @return integer 0 on a successfully exit, another integer value otherwise
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int main(int argc, char *argv[]){
	/* Variable declarations */
	struct gengetopt_args_info args_info;						// structure for the command line parameters processing
	FILE *log_file = NULL;										// log file reference
	int result = 0;
	/* Main code */

	system("clear");

	// Initializes the command line parser and check for the application parameters
	if (cmdline_parser(argc,argv,&args_info) != 0){
		DEBUG("\nInvalid parameters");
		exit(M_INVALID_PARAMETERS);
	}

	// Remove the duplicated algorithms given by the program arguments
	remove_duplicated_algorithms(&args_info);

	// Enable daemon mode if the flag was sent
	daemonize(args_info);

	// Enable log mode if the parameter was sent
	log_file=lognize(args_info);

	// Register the signal handler
	register_signal_handlers();

	// print the header message
	print_log_header(args_info, argc, argv);


	// Let's process the directory
	if((result = processDir(args_info, argc, argv))!=TRUE){
		printf("\nThe processing failed with the error %d\n",result);
		result = M_PROCESSING_FAILED;
	}

	// If we are logging to a file, let's restore the default behavior
	if (args_info.log_given && log_file!=NULL){
		close_log_file(log_file);
	}

	// Free the command line parser memory
	cmdline_parser_free(&args_info);

	return result;
}

/**
 * @brief Print the output log to a file or to the stdout (based on the application parameters)
 * @param args_info struct gengetopt_args_info with the parameters given to the application
 * @param argc integer with main argument count
 * @param argv with the arguments
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void print_log_header(struct gengetopt_args_info args_info, int argc, char *argv[]){
	int a=0;
	char* aux=NULL;

	printf("\n# sorter\n# options: ");
	// command-line options
	for(a=1;a<argc;a++){
		printf("%s ",argv[a]);
	}
	//Print the date
	aux = get_current_time("@%Y-%m-%d %Hh%M", strlen("@2009-10-09 15h30"));
	printf("\n# start date: %s\n",aux);
	free(aux);
	aux=NULL;

	// For the host name
	if((aux=malloc((MAXCHARS+1)*sizeof(char)))==NULL){
		ERROR(M_FAILED_MEMORY_ALLOCATION,"\nMemory allocation failed for host name");
	}

	if(gethostname(aux, MAXCHARS)!=0){
		ERROR(M_GETHOSTNAME_FAILED,"\nCan't get the host name");
	}
	printf("# machine: %s, user: %s\n",aux,getlogin());
	free(aux);
	aux=NULL;

	// PID
	printf("# PID: %d\n",getpid());

	// Files to process
	printf("# Files to process: %d\n",count_dir_items(args_info.input_arg));

	// Algorithms to execute
	printf("# Algorithms to execute: %d\n",args_info.serial_algorithm_given);

	printf("#-----------------------\n");
}

/**
 * @brief Process the input directory items
 * @param args_info the gengetopt_args_info object
 * @param argc integer with main argument count
 * @param argv with the arguments
 * @return integer TRUE if the directory exists, FALSE otherwise
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int processDir(struct gengetopt_args_info args_info, int argc, char *argv[]){
	unsigned int a=0;																	// auxiliary integer for the algorithms parameters loop
	int files_counter=0, files_total=0, algorithm_counter=0;							// integers to store some counters
	int result;																			// auxiliary result integer
	char *input_filename=NULL, *output_filename;										// to store the input and output filenames
	char *model_name=NULL;																// to store the computer model name
	char md5sum_char[MD5SUMCHARS+1];													// string to store the md5 sum
	struct dirent *dirItem=NULL; 														// to reference a directory item
	struct stat fileDetails;															// to reference the file details
	struct sockaddr_in udp_time_server_addr, udp_results_server_addr;					// to store server addresses
	DIR *dir = NULL;																	// to reference the directory descriptor
	FILE_LINES_T *flines = NULL, *sorted_flines=NULL;									// to store the file lines
	ALGORITHM_STAT_T* stat=NULL;														// to store the statistical data of the sort operation
	ALGORITHM_FUNC algorithm_function=NULL;												// to store the algorithm function to use on the sort operation
	CONTROLLER_STAT_T controller_stat;													// to store the statistical controller control
	REMOTE_UDP_REQUEST_T rur_time, rur_results;											// to store the UDP request data for the UDP time server

	// Begin of the function code
	(void) argc; // silence the unused warning

	// Verify if we really have a input and output parameters
	if (args_info.input_given && args_info.output_given && args_info.serial_algorithm_given>0){
		// Initializes and, if requested, creates a socket UDP for the time server
		rur_time.sock_fd = -1;
		rur_time.server_addr = &udp_time_server_addr;
		if(args_info.time_server_addr_given && args_info.time_server_port_given){
			if((result = initialize_udp_connection(&rur_time, args_info.time_server_addr_arg, args_info.time_server_port_arg))!=0){
				return result;
			}
		}
		// Initializes and, if requested, creates a socket UDP for the results server
		rur_results.sock_fd = -1;
		rur_results.server_addr = &udp_results_server_addr;
		if(args_info.stats_server_given && args_info.stats_port_given){
			if((result = initialize_udp_connection(&rur_results, args_info.stats_server_arg, args_info.stats_port_arg))!=0){
				return result;
			}
			if((model_name = get_model_name())==NULL){
				MY_DEBUG("\nError while retrieving the computer model name info\n");
				return M_RETRIEVE_MODEL_NAME_ERROR;
			}
		}

		// Initializes the controller
		result = initializes_controller_stat(&controller_stat, argv[0], count_dir_items(args_info.input_arg), args_info.serial_algorithm_given);
		// Test for errors
		switch(result){
			case M_SEMSET_FAILED_FOR_DATA:
				shmdt(controller_stat.stats);
			case M_SHMAT_FAILED_FOR_DATA_STATS:
				sem_delete(controller_stat.sem_data_id);
			case M_SEMCREATE_FAILED_FOR_DATA:
				shmctl(controller_stat.shm_data_id, IPC_RMID, 0);
			case M_SHMGET_FAILED_FOR_DATA:
			case M_FTOK_FAILED_FOR_DATA:
				shmdt(controller_stat.control_data);
			case M_SHMAT_FAILED_FOR_CONTROL:
			case M_SEMSET_FAILED_FOR_CONTROL:
				sem_delete(controller_stat.sem_control_id);
			case M_SEMCREATE_FAILED_FOR_CONTROL:
				shmctl(controller_stat.shm_control_id, IPC_RMID, 0);
			case M_SHMGET_FAILED_FOR_CONTROL_ID:
			case M_FTOK_FAILED_FOR_CONTROL_KEY:
				return result;
		}

		// Verify if we can open the output directory
		if(dir_exists(args_info.output_arg)!=TRUE){
			ERROR(M_OPEN_DIR_FAILED,"\nError while open the output directory %s", args_info.output_arg);
		}
		closedir(dir);

		// Verify if we can open the input directory
		if((dir = opendir(args_info.input_arg))==NULL){
			ERROR(M_OPEN_DIR_FAILED,"\nError while open the input directory %s", args_info.input_arg);
		}

		// Set selected algorithms variable
		for(a=0; a<args_info.serial_algorithm_given; a++){
			if(a!=0){
				sprintf(controller_stat.control_data->selected_algorithms, "%s%s", controller_stat.control_data->selected_algorithms, ", ");
			}
			sprintf(controller_stat.control_data->selected_algorithms, "%s%s", controller_stat.control_data->selected_algorithms, args_info.serial_algorithm_orig[a]);
		}

		// Give the green light for the shared memory access by another process
		release_controller_stat(&controller_stat);

		// Allocate memory for statistical data
		stat = allocate_memory_for_stat(NULL,NULL);

		// Count the number of files to process
		files_total = count_dir_items(args_info.input_arg);

		// Reads all the items on the directory
		while(_sigint_time == NULL && ((dirItem = readdir(dir))!=NULL)){
			output_filename = path_filename(args_info.output_arg, dirItem->d_name);
			input_filename = path_filename(args_info.input_arg, dirItem->d_name);

			// Reads the file attributes
			if(lstat(input_filename, &fileDetails)==0){
				//if item is a regular file
				if((fileDetails.st_mode & S_IFREG)!=0){

					MY_DEBUG("\nLoading the file %s (%lld bytes)... \n", input_filename, (long long) fileDetails.st_size);
					// Read the file to memory
					if((flines = read_file(input_filename, MAXCHARS))!=NULL){
						MY_DEBUG("Loading OK!\n");
						files_counter++;
						algorithm_counter=0;

						// Loop between the specified algorithms
						for(a=0; a<args_info.serial_algorithm_given; a++){
							// Count current algorithm
							algorithm_counter++;

							MY_DEBUG("Sorting the %d lines", flines->num_lines);

							// Set the algorithm function and resets the statistical data
							if(args_info.serial_algorithm_arg[a]==serial_algorithm_arg_bubble){
								MY_DEBUG(" using the %s algorithm... \n", "bubble sort");
								// Sets the sort function for the bubble sort algorithm
								algorithm_function = bubble_sort;
								// Resets the statistical data for the current sort process
								reset_stat(stat, dirItem->d_name, "bubble");
							}else if(args_info.serial_algorithm_arg[a]==serial_algorithm_arg_merge){
								MY_DEBUG(" using the %s algorithm... \n", "merge sort");
								// Sets the sort function for the merge sort algorithm
								algorithm_function = merge_sort;
								// Resets the statistical data for the current sort process
								reset_stat(stat, dirItem->d_name, "merge");
							}else if(args_info.serial_algorithm_arg[a]==serial_algorithm_arg_quick){
								MY_DEBUG(" using the %s algorithm... \n", "quick sort");
								// Sets the sort function for the quick sort algorithm
								algorithm_function = quick_sort;
								// Resets the statistical data for the current sort process
								reset_stat(stat, dirItem->d_name, "quick");
							}else if(args_info.serial_algorithm_arg[a]==serial_algorithm_arg_shell){
								MY_DEBUG(" using the %s algorithm... \n", "shell sort");
								// Sets the sort function for the shell sort algorithm
								algorithm_function = shell_sort;
								// Resets the statistical data for the current sort process
								reset_stat(stat, dirItem->d_name, "shell");
							}else{
								ERROR(M_UNKNOWN_ALGORITHM, "Unknown algorithm\n");
							}

							// Sort the data
							if((sorted_flines = sort_lines(clone_of_lines(flines), algorithm_function, stat, rur_time))!=NULL){
								// Check if the output file exists
								if(file_exists(output_filename, "r")!=TRUE){
									// if not, write the file
									MY_DEBUG("Sort OK!\nSaving the result to %s... \n", output_filename);
									if(save_file(output_filename, sorted_flines)!=TRUE){
										ERROR(M_FILE_OUTPUT_FAILED, "\nError creating the output file %s", output_filename);
									}
									MY_DEBUG("Save OK!\n");
								}else{
									// if exists, skip the output file creation
									MY_DEBUG("File %s exists! Skipping file creation.\n", output_filename);
								}
								// Calculates the md5 sum of the output_filename and store it on the md5sum_char string
								md5sum(md5sum_char,output_filename);

								// Output the results or log it to the log file
								printf("[%d/%d]%s:[%d/%d]%s:%.0f\n", files_counter, files_total, stat->filename, algorithm_counter, args_info.serial_algorithm_given, stat->algorithm, stat->time);
								// Append this new data to the shared memory
								append_stat(&controller_stat, stat, _sigint_time!=NULL);
								// Send result to the UDP results server
								send_udp_result(rur_results, stat, NICKNAME, model_name, md5sum_char);

								// free the used memory for the lines clone
								free_memory_of_clone_of_lines(sorted_flines);
							}else{
								ERROR(M_SORT_FAILED, "\nError sorting the file %s", input_filename);
							}
						}
						// free the used memory for the file lines
						free_memory_of_lines(flines);
					}
				}
			}else{
				ERROR(M_FILE_ATTRIBUTES_READ_FAILED, "\nError reading the file attributes from %s", input_filename);
			}
			// free the others resources
			free(input_filename);
			input_filename=NULL;
			free(output_filename);
			output_filename=NULL;
		} //end while
		closedir(dir);

		// Free the statistical used memory
		free_memory_of_stat(stat);

		// If we have a _sigint_time, the operation was interrupted by the user
		if(_sigint_time!=NULL){
			printf("\nOperation interrupted by user %s\n",_sigint_time);
			free(_sigint_time);
			_sigint_time=NULL;
		}

		// Flag for any listener to exit
		lock_controller_stat(&controller_stat);
		controller_stat.control_data->to_exit=TRUE;
		release_controller_stat(&controller_stat);
		release_new_data(&controller_stat);

		// If we have a time socket open, close it
		if(rur_time.sock_fd>-1){
			close(rur_time.sock_fd);
		}
		// If we have a results socket open, close it
		if(rur_results.sock_fd>-1){
			close(rur_results.sock_fd);
		}

		// Free the allocated memory for the model_name
		if(model_name!=NULL){
			free(model_name);
		}
	}

	// Wait for the other showStats
	wait_for_exit_unlock(&controller_stat);
	// Remove the controller, shared memory and semaphores
	result = remove_controller_stat(&controller_stat);
	// Test for errors
	switch(result){
		case M_SHMDT_FAILED:
			ERROR(result, "\nError while detaching shared control memory");
		case M_SHMCTL_FAILED:
			ERROR(result, "\nError while removing the shared memory segment");
		case M_SEMDELETE_FAILED:
			ERROR(result, "\nError while deleting the semaphore");
	}

	return TRUE;
}

/**
 * @brief Handle the signals sent to the application
 * @param signal integer with the signal to be handled
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void handle_signal(int signal) {
	int aux;

	/* Copy the value of the global variable errno */
	aux = errno;
	if(signal==SIGINT){
		_sigint_time = get_current_time("@%Y-%m-%d %Hh%M", strlen("@2009-10-08 16h02"));
		MY_DEBUG("Signal SIGINT received!\n");
	}
	/* Restore the value for the global variable errno */
	errno = aux;
}

/**
 * @brief Register the signal handler function
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void register_signal_handlers(void){
	struct sigaction action;									// structure for the signal handling

	// Signal handling function
	action.sa_handler = handle_signal;
	// Mask without signals - won't block the signals
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	// Recover the blocked calls
	action.sa_flags |= SA_RESTART;
	// Specify the signals that will be handled
	if(sigaction(SIGINT, &action, NULL) < 0){
		ERROR(M_SIGACTION_SIGINT_FAILED, "Action for signal SIGINT failed!\n");
	}
}

/**
 * @brief Enable the daemon mode if such setting was request through command line parameter
 * @param args_info struct gengetopt_args_info with the parameters given to the application
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void daemonize(struct gengetopt_args_info args_info){
	pid_t pid;													// pid for the daemon
	// If the flag daemon was sent
	if (args_info.daemon_given){
		// And we have a output log filename parameter
		if(args_info.log_given){
			// Let's fork the main program
			switch(pid=fork()){
				case -1:
					ERROR(M_FORK_FAILED, "\nFork for the daemon mode failed!\n");
					break;
				case 0: //Child proceeds
					break;
				default: //Parent exit
					// Free the command line parser memory
					cmdline_parser_free(&args_info);
					exit(0);
			}
		}else{
			// fail safe that should happen; this should be verified by the gengetopt code
			printf("\nDaemon mode requested but no log file given! Exiting...\n");
			exit(M_DAEMON_BUT_NO_LOG_FILE);
		}
	}
}

/**
 * @brief Enable the log mode if such setting was request through command line parameter
 * @param args_info struct gengetopt_args_info with the parameters given to the application
 * @return FILE* descriptor if a file was opened, NULL otherwise
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
FILE* lognize(struct gengetopt_args_info args_info){
	FILE *log_file = NULL;
	// If we have a log parameter, let's redirect the stdout directly to the file
	if (args_info.log_given){
		if((log_file=open_log_file(args_info.log_arg,"a"))==NULL){
			printf("\nUnable to log to the %s file...", args_info.log_arg);
		}
	}
	return log_file;
}

/**
 * @brief Removes the duplicated algorithms in the serial_algorithm_arg array from the gengetopt_args_info structure
 * @param args_info struct gengetopt_args_info with the algorithms to normalize
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void remove_duplicated_algorithms(struct gengetopt_args_info *args_info){
	enum enum_serial_algorithm serial_algorithm_arg[args_info->serial_algorithm_given];
	char* serial_algorithm_orig[args_info->serial_algorithm_given];
	unsigned int a, b, total=0;
	int exits=FALSE;

	total=0;
	// Loop trough the given algorithms
	for(a=0; a<args_info->serial_algorithm_given; a++){
		exits=FALSE;
		// Verify if the algorithm already exists in the unique algorithms array, and if it exists, free it's memory
		for(b=0; b<total; b++){
			if(args_info->serial_algorithm_arg[a]==serial_algorithm_arg[b]){
				exits=TRUE;
				// Free the associated memory
				free(args_info->serial_algorithm_orig[a]);
				args_info->serial_algorithm_orig[a]=NULL;

			}
		}
		// If not, add the current algorithm to the unique algorithms array
		if(exits!=TRUE){
			serial_algorithm_arg[total] = args_info->serial_algorithm_arg[a];
			serial_algorithm_orig[total] = args_info->serial_algorithm_orig[a];
			total++;
		}
	}

	// Copy from the unique algorithms array to the gengetopt_args_info structure
	for(b=0; b<args_info->serial_algorithm_given; b++){
		if(b<total){
			args_info->serial_algorithm_arg[b]=serial_algorithm_arg[b];
			args_info->serial_algorithm_orig[b]=serial_algorithm_orig[b];
		}
	}
	args_info->serial_algorithm_given=total;
}
