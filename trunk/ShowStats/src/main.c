/**
* \mainpage
* The ShowStats is an application to show the data from Sorter daemon using IPC. It serves a webserver to allow the statistics access with the HTTP protocol
* 
* 
* @file main.c
* @brief Main source file for the ShowStats program
* @date 2009-10-28 File creation
* @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "3rd/debug.h"
#include "3rd/semaforos.h"
#include "3rd/showstats_options.h"


#include "includes/definitions.h"
#include "includes/aux.h"
#include "includes/commonlib.h"
#include "includes/showstatslib.h"
#include "includes/webslib.h"
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
	/* Variable declarations */
	struct gengetopt_args_info args_info;
	FILE *csv_file = NULL;
	CONTROLLER_STAT_T controller_stat;							// to store the statistical controller control
	int result;													// to store a result of an operation
	pthread_t t_web_server;
	pthread_mutex_t web_server_mutex;
	WEB_SERVER_PARAMS_T web_server_params;
	
	/* Main code */
	system("clear");

	// Initializes the command line parser and check for the application parameters
	if (cmdline_parser(argc,argv,&args_info) != 0){
		DEBUG("\nInvalid parameters");
		exit(M_INVALID_PARAMETERS);
	}

	// If we have a HTTP parameter
	if (args_info.http_given){
		if(args_info.http_arg<PORT_RANGE_MIN || args_info.http_arg>PORT_RANGE_MAX){
			MY_DEBUG("\nPORT_OUT_OF_RANGE\n");
			printf("The port %d is out of the allowed range port numbers. Port to listen should be a number between %d and %d\n", args_info.http_arg, PORT_RANGE_MIN, PORT_RANGE_MAX);
			exit(M_PORT_OUT_OF_RANGE);
		}
		// Initialize the mutex for the thread
		if (pthread_mutex_init(&web_server_mutex, NULL) != 0) {
			ERROR(M_PTHREAD_MUTEX_INIT_FAILED, "\nWeb Server mutex initialization failed.\n");
		}
		web_server_params.mutex = &web_server_mutex;

		web_server_params.content = NULL;
		web_server_params.content = update_content(&web_server_params, web_server_params.content,"", "", "");
		// Activate the web server if requested by the command line
		webnize(args_info, HTTP_MAXIMUM_CONNECTIONS, &t_web_server, &web_server_params);
	}

	// Initializes the controller and terminate the program on errors
	if((result = parse_initializes_controller_stat_result(initializes_controller_stat(&controller_stat, args_info.id_arg), &controller_stat))!=0){
		MY_DEBUG("\nThe processing failed with the error %d\n",result);
		switch(result){
			case M_SHMGET_FAILED_FOR_CONTROL_ID:
				printf("\nUnable to access to the shared memory; is the %s running?\n",args_info.id_arg);
				break;
			default:
				printf("\nThe processing failed with the error %d\n",result);
		}
		shutdown_web_server(&t_web_server, &web_server_params);
		cmdline_parser_free(&args_info);
		return result;
	}

	// Let's try to register this show_stats in the shared memory
	register_show_stats(&controller_stat);

	// Open a CVS file if such operation was requested or print some info
	if((csv_file = csvnize(args_info))==NULL){
		print_header(controller_stat);
	}

	// Ok, let's show some stats
	if (args_info.http_given){
		show_stats(&controller_stat, &web_server_params);
	}else{
		show_stats(&controller_stat, NULL);
	}

	// Close the csv file
	if(csv_file!=NULL){
		close_log_file(csv_file);
	}

	if (args_info.http_given){
		// Shutdown the web server thread
		shutdown_web_server(&t_web_server, &web_server_params);
	}

	// Free the command line parser memory
	cmdline_parser_free(&args_info);

	return 0;
}

/**
 * @brief Removes the duplicated algorithms in the serial_algorithm_arg array from the gengetopt_args_info structure
 * @param controller_stat with the control structure for stats
 * @param web_server_params with the thread web server parameters
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void show_stats(CONTROLLER_STAT_T* controller_stat, WEB_SERVER_PARAMS_T *web_server_params){
	int result, aux, hold=TRUE, counter=0;
	char line[MAXCHARS];

	if(web_server_params!=NULL){
		web_server_params->content = update_content(web_server_params, web_server_params->content, "", "", "");
	}
	do{
		// Critical section bellow
		wait_for_new_data(controller_stat);
		lock_data_for_access(controller_stat);
		aux = controller_stat->control_data->index_stat-1;
		// If new data was added (the Sorter flag as an update a exit request)
		if(counter<=aux){
			printf("%s,%d,%s,%d,%d,%.0f\n",controller_stat->stats[counter].filename, controller_stat->stats[counter].nlines, controller_stat->stats[counter].algorithm, controller_stat->stats[counter].niterations, controller_stat->stats[counter].nswaps, controller_stat->stats[counter].time);
			if(web_server_params!=NULL){
				snprintf(line, (MAXCHARS-1)*sizeof(char), "<tr><td>%s</td><td>%d</td><td>%s</td><td>%d</td><td>%d</td><td>%.0f</td></tr>",controller_stat->stats[counter].filename, controller_stat->stats[counter].nlines, controller_stat->stats[counter].algorithm, controller_stat->stats[counter].niterations, controller_stat->stats[counter].nswaps, controller_stat->stats[counter].time);
				line[MAXCHARS]='\0';
				web_server_params->content = update_content(web_server_params, web_server_params->content, web_server_params->content, line, "");
			}
			counter++;
			// If we still have more data available, release the semaphore for the next iteration
			if(counter<aux){
				release_new_data(controller_stat);
			}
		}
		// If is to exit, flag for the exit of the loop on the next check
		if(controller_stat->control_data->to_exit==TRUE && counter>aux){
			hold=FALSE;
		}
		release_data_for_access(controller_stat);
	}while(hold==TRUE);

	// Detach from the shared memory
	result = remove_controller_stat(controller_stat);
	if(result==M_SHMDT_FAILED){
		ERROR(result, "\nError while detaching shared control memory");
	}
	// Remove this process from the "listener list"
	remove_listener(controller_stat);
}

/**
 * @brief Redirect output to a file if such setting was request through command line parameter
 * @param args_info struct gengetopt_args_info with the parameters given to the application
 * @return FILE* descriptor if a file was opened, NULL otherwise
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
FILE* csvnize(struct gengetopt_args_info args_info){
	FILE *csv_file = NULL;
	char* export_filename=NULL;

	// If we have a export parameter, let's redirect the stdout directly to the file
	if (args_info.export_given){
		// If the user forgot to add the extension
		if(ends_with(args_info.export_arg,SHOW_STATS_FILE_EXPORT_EXTENSION)!=TRUE){
			// Allocate memory for the filename with the extension
			if((export_filename=malloc(sizeof(char)*(strlen(args_info.export_arg)+strlen(SHOW_STATS_FILE_EXPORT_EXTENSION)+1)))==NULL){
				ERROR(M_FAILED_MEMORY_ALLOCATION,"\nMemory allocation failed for the filename %s%s", args_info.export_arg, SHOW_STATS_FILE_EXPORT_EXTENSION);
			}
			// Adds the extension to the filename
			sprintf(export_filename,"%s%s",args_info.export_arg,SHOW_STATS_FILE_EXPORT_EXTENSION);
		}else{
			// Allocate memory for the filename with the extension
			if((export_filename=malloc(sizeof(char)*(strlen(args_info.export_arg)+1)))==NULL){
				ERROR(M_FAILED_MEMORY_ALLOCATION,"\nMemory allocation failed for the filename %s", args_info.export_arg);
			}
			// Copy to the filename
			sprintf(export_filename,"%s",args_info.export_arg);
		}
		// Open the log file
		if((csv_file=open_log_file(export_filename,"w"))==NULL){
			printf("\nUnable to export to the %s file...", export_filename);
		}
		// Free the memory allocated for the filename
		if(export_filename!=NULL){
			free(export_filename);
		}
	}
	return csv_file;
}

/**
 * @brief If requested, starts the web server on the specified port
 * @param args_info with the application arguments structure
 * @param connections with maximum number of connections to handle
 * @param t_web_server with the web server thread
 * @param web_server_params with the thread web server parameters
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void webnize(struct gengetopt_args_info args_info, int connections, pthread_t *t_web_server, WEB_SERVER_PARAMS_T *web_server_params){
	char port[6];
	if (args_info.http_given){
		if(args_info.http_arg<PORT_RANGE_MIN || args_info.http_arg>PORT_RANGE_MAX){
			MY_DEBUG("\nPORT_OUT_OF_RANGE\n");
			printf("The port %d is out of the allowed range port numbers. Port to listen should be a number between %d and %d\n", args_info.http_arg, PORT_RANGE_MIN, PORT_RANGE_MAX);
			exit(M_PORT_OUT_OF_RANGE);
		}
		sprintf(port, "%d", args_info.http_arg);
		web_server_params->content = update_content(web_server_params, web_server_params->content,"<tr><td colspan='6'><h1>", "Server is initializing...", "<h1></td></tr>");
		// Create the web server on the specified port
		create_web_server(port, connections, t_web_server, web_server_params);
		web_server_params->content = update_content(web_server_params, web_server_params->content,"<tr><td colspan='6'><h1>", "Server is initialized. Waiting for data...", "</h1></td></tr>");
	}
}

/**
 * @brief Print a header
 * @param controller_stat with the control structure for stats
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void print_header(CONTROLLER_STAT_T controller_stat){
	char* date_of_the_experiment=NULL;

	date_of_the_experiment = get_current_time("@%Y-%m-%d %Hh%M", strlen("@2009-10-09 15h30"));
	printf("# showStats – sorter benchmark\n# Selected algorithms: %s\n# Date: %s\n# filename,nlines,algorithm,niterations,nswaps,time\n", controller_stat.control_data->selected_algorithms, date_of_the_experiment);
	free(date_of_the_experiment);
	date_of_the_experiment=NULL;
}

/**
 * @brief Register the show_stats as a listener in the shared memory
 * @param controller_stat with the control structure for stats
 *
 * @author Cláudio Esperança <2070030@student.estg.ipleiria.pt>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void register_show_stats(CONTROLLER_STAT_T* controller_stat){
	// Let's try to lock the controller for this process (and wait for the controller release by the sorter)
	lock_controller_stat(controller_stat);
	if(controller_stat->control_data->locked==TRUE){
		printf("\nAnother process has locked the shared memory segment (another showStats listening?)\n");
		// Detach from the shared memory
		remove_controller_stat(controller_stat);
		// Release the controller
		release_controller_stat(controller_stat);
		exit(M_LOCKED_CONTROL_STATS);
	}
	controller_stat->control_data->locked = TRUE;
	// Release the controller
	release_controller_stat(controller_stat);

	// Adds this process as a listener
	add_listener(controller_stat);
}
