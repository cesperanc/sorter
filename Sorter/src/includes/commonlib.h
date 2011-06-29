/**
* @file commonlib.h
* @brief Header file for the common lib functions
* @date 2009/12/08 File creation
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/



#ifndef COMMONLIB_H_
#define COMMONLIB_H_

/**
 * @brief Type declaration to a structure to store the statistical data from a sort algorithm
 */
typedef struct algorithm_stat {
	char* filename; 											/**< @brief reference to the algorithm name used on the sort */
	int nlines;													/**< @brief number of lines processed */
	char* algorithm; 											/**< @brief reference to the algorithm name used on the sort */
	int niterations;											/**< @brief reference to the number of iterations. */
	int nswaps;													/**< @brief reference to the number of swaps. */
	float time;													/**< @brief reference to the time of the operation. */
} ALGORITHM_STAT_T;

/**
 * @brief Type declaration to a structure to store the statistical data from a sort algorithm in the shared memory
 */
typedef struct shared_algorithm_stat {
	char filename[MAXCHARS]; 									/**< @brief reference to the algorithm name used on the sort */
	int nlines;													/**< @brief number of lines processed */
	char algorithm[MAXCHARS]; 									/**< @brief reference to the algorithm name used on the sort */
	int niterations;											/**< @brief reference to the number of iterations. */
	int nswaps;													/**< @brief reference to the number of swaps. */
	float time;													/**< @brief reference to the time of the operation. */
} SHARED_ALGORITHM_STAT_T;

/**
 * @brief Type declaration to a structure to store the control data from the shared memory between the sorter and the showStats
 */
typedef struct control_stat {
	int number_of_files;										/**< @brief number of files to be processed */
	int number_of_algorithms;									/**< @brief number of algorithms to process the files */
	char selected_algorithms[MAXCHARS];							/**< @brief char[] with algorithms used in sorting process */
	int to_exit; 												/**< @brief control if the sorter process is flagged to exit */
	int locked; 												/**< @brief control if any showStats process is listening */
	int index_stat;												/**< @brief index of the current statistical data available */
	//SHARED_ALGORITHM_STAT_T* stats;								/**< @brief sorter sorting algorithm statistical data */
} CONTROL_STAT_T;

/**
 * @brief Type declaration to a structure to store the a controller for the statistical data control
 */
typedef struct controller_stat {
	CONTROL_STAT_T* control_data;								/**< @brief reference to the control data structure */
	SHARED_ALGORITHM_STAT_T* stats;								/**< @brief sorter sorting algorithm statistical data */
	int shm_control_id;											/**< @brief reference to the shared memory for the control data */
	int shm_data_id;											/**< @brief reference to the shared memory for the data */
	int sem_control_id;											/**< @brief reference to the semaphore for the access to shared memory control data */
	int sem_data_id;											/**< @brief reference to the semaphore for the access to shared memory data */
} CONTROLLER_STAT_T;

void release_controller_stat(CONTROLLER_STAT_T*);
void lock_controller_stat(CONTROLLER_STAT_T*);
void release_data_for_access(CONTROLLER_STAT_T*);
void lock_data_for_access(CONTROLLER_STAT_T*);
void release_new_data(CONTROLLER_STAT_T*);
void wait_for_new_data(CONTROLLER_STAT_T*);

#endif /* COMMONLIB_H_ */
