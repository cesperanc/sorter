/**
* @file showstatslib.c
* @brief source file for the showStats functions
* @date 2009/12/12 File creation
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "../3rd/debug.h"
#include "../3rd/semaforos.h"
#include "definitions.h"
#include "aux.h"
#include "commonlib.h"
#include "showstatslib.h"

/**
 * @brief Initializes a statistical control controller and access the shared statistical control memory segment
 * @param controller_stat with the reference to the statistical control controller to initialize
 * @param path_name path name for the System V IPC key
 * @return integer 0 if the controller was successfully initialized, the error code otherwise
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int initializes_controller_stat(CONTROLLER_STAT_T* controller_stat, char* path_name){
	key_t control_key;													/**< @brief store a System V IPC key for the control */
	key_t data_key;														/**< @brief store a System V IPC key for the data */
	struct shmid_ds info;

	// Generate a shared memory key for this program
	if((control_key = ftok(path_name, 'c'))==-1){
		MY_DEBUG("\nShared memory key generation for the control data failed.\n");
		return M_FTOK_FAILED_FOR_CONTROL_KEY;
	}
	// Access the shared memory zone for the control data
	if((controller_stat->shm_control_id = shmget(control_key, 0, 0))==-1){
		MY_DEBUG("\nError while creating the shared memory for the control data with the key [%x]. Is another %s running? If not, remove the shared memory segment(s)\n", control_key, path_name);
		return M_SHMGET_FAILED_FOR_CONTROL_ID;
	}
	info.shm_nattch = 0; // to ignore the uninitialized warning
	if (shmctl(controller_stat->shm_control_id, IPC_STAT, &info) == -1){
		MY_DEBUG("\nError while getting information about the memory segment.\n", control_key, path_name);
		return M_SHMCTL_FAILED;
	}else{
		switch(info.shm_nattch){
			case 0:
				printf("\nThere is no program attached to the shared memory (Is the %s running?)\n", path_name);
				return M_NO_PROGRAM_ATTACHED_TO_MEMORY;
			case 1:
				break;
			default:
				printf("\nAnother process (%d) is already listening\n", info.shm_lpid);
				return M_MAXIMUM_SHMAT_CONTROL_STATS;
		}
	}
	// Attach shared memory for the control to this process
	if((controller_stat->control_data = shmat(controller_stat->shm_control_id, NULL, 0))==(void *)-1){
		MY_DEBUG("\nError while attaching on the shared memory segment for the control data\n");
		return M_SHMAT_FAILED_FOR_CONTROL;
	}
	// Associate the semaphore for the shared memory zone with the controller
	if((controller_stat->sem_control_id = semget(control_key, 0, 0))==-1){
		MY_DEBUG("\nError while getting the semaphore for the access to the shared memory control data. Is the %s running?", path_name);
		return M_SEMGET_FAILED_FOR_CONTROL;
	}

	// Generate a shared memory key for the data key
	if((data_key = ftok(path_name, 'd'))==-1){
		MY_DEBUG("\nShared memory key generation for the data failed.\n");
		return M_FTOK_FAILED_FOR_DATA;
	}
	// Gets a shared memory zone for the data
	if((controller_stat->shm_data_id = shmget(data_key, 0, 0))==-1){
		MY_DEBUG("\nError while getting the shared memory for the data with the key [%x].\n", data_key);
		return M_SHMGET_FAILED_FOR_DATA;
	}
	// Attach shared memory for the data to this process (in read only mode)
	if((controller_stat->stats = shmat(controller_stat->shm_data_id, NULL, SHM_RDONLY))==(void *)-1){
	//if((controller_stat->control_data->stats = shmat(controller_stat->shm_data_id, NULL, 0))==(void *)-1){
		MY_DEBUG("\nError while attaching on the shared memory segment for the data\n");
		return M_SHMAT_FAILED_FOR_DATA_STATS;
	}
	// Associate the semaphore for the shared memory zone for the data
	if((controller_stat->sem_data_id = semget(data_key, 0, 0))==-1){
		MY_DEBUG("\nError while getting the semaphore for the access to the shared memory control data. Is the %s running?", path_name);
		return M_SEMGET_FAILED_FOR_DATA;
	}

	return 0;
}

/**
 * @brief detach the statistical control from the shared memory
 * @param controller_stat with the reference to the statistical control controller to lock access
 * @return integer 0 if the controller was successfully initialized, the error code otherwise
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int remove_controller_stat(CONTROLLER_STAT_T* controller_stat){
	// Update the lock
	lock_controller_stat(controller_stat);
	controller_stat->control_data->locked=FALSE;
	release_controller_stat(controller_stat);

	// Detach from the shared memory
	if (shmdt(controller_stat->stats) == -1){
		return M_SHMDT_FAILED;
	}
	//controller_stat->control_data->stats = NULL;
	// Detach from the shared memory
	if (shmdt(controller_stat->control_data) == -1){
		return M_SHMDT_FAILED;
	}
	return 0;
}

/**
 * @brief add a listener locking the exit of sorter
 * @param controller_stat with the reference to the statistical control controller to release access
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void add_listener(CONTROLLER_STAT_T* controller_stat){
	if(sem_down(controller_stat->sem_control_id, MUTEX_EXIT)==-1){
		ERROR(M_SEMDOWN_FAILED, "\nError while adding the listener");
	}
}

/**
 * @brief remove a listener so that Sorter can exit
 * @param controller_stat with the reference to the statistical control controller to release access
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void remove_listener(CONTROLLER_STAT_T* controller_stat){
	if(sem_up(controller_stat->sem_control_id, MUTEX_EXIT)==-1){
		ERROR(M_SEMUP_FAILED, "\nError while remove a listener");
	}
}

/**
 * @brief Parses a result from the initializes_controller_stat function, and free the resources on error
 * @param result with the integer value result of the initializes_controller_stat function
 * @param controller_stat with the reference to the statistical control controller to initialize
 * @return integer with the same value sended as argument
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int parse_initializes_controller_stat_result(int result, CONTROLLER_STAT_T* controller_stat){
	switch(result){
		case M_SEMGET_FAILED_FOR_DATA:
			shmdt(controller_stat->stats);
		case M_SHMAT_FAILED_FOR_DATA_STATS:
			shmctl(controller_stat->shm_data_id, IPC_RMID, 0);
		case M_SHMGET_FAILED_FOR_DATA:
		case M_FTOK_FAILED_FOR_DATA:
			sem_delete(controller_stat->sem_control_id);
		case M_SEMGET_FAILED_FOR_CONTROL:
			shmdt(controller_stat->control_data);
		case M_SHMAT_FAILED_FOR_CONTROL:
		case M_NO_PROGRAM_ATTACHED_TO_MEMORY:
		case M_MAXIMUM_SHMAT_CONTROL_STATS:
		case M_SHMCTL_FAILED:
			shmctl(controller_stat->shm_control_id, IPC_RMID, 0);
		case M_SHMGET_FAILED_FOR_CONTROL_ID:
		case M_FTOK_FAILED_FOR_CONTROL_KEY:
			break;
	}
	return result;
}

