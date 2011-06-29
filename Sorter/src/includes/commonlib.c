/**
* @file commonlib.c
* @brief contains the common functions and between sorter and the showStats
* @date 2009/12/08 File creation
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#include <sys/types.h>

#include "../3rd/debug.h"
#include "../3rd/semaforos.h"
#include "definitions.h"
#include "commonlib.h"

/**
 * @brief release the controller for access by another client
 * @param controller_stat with the reference to the statistical control controller to release access
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void release_controller_stat(CONTROLLER_STAT_T* controller_stat){
	if(sem_up(controller_stat->sem_control_id, MUTEX_CONTROL_STATS)==-1){
		ERROR(M_SEMUP_FAILED, "\nError while releasing a semaphore");
	}
}

/**
 * @brief lock the controller forbidding the access by another client
 * @param controller_stat with the reference to the statistical control controller to lock access
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void lock_controller_stat(CONTROLLER_STAT_T* controller_stat){
	if(sem_down(controller_stat->sem_control_id, MUTEX_CONTROL_STATS)==-1){
		ERROR(M_SEMDOWN_FAILED, "\nError while locking a semaphore");
	}
}

/**
 * @brief release the access for the data
 * @param controller_stat with the reference to the statistical control controller to release access
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void release_data_for_access(CONTROLLER_STAT_T* controller_stat){
	if(sem_up(controller_stat->sem_data_id, MUTEX_DATA_ACCESS)==-1){
		ERROR(M_SEMUP_FAILED, "\nError while releasing a semaphore");
	}
}

/**
 * @brief lock the access to the data
 * @param controller_stat with the reference to the statistical control controller to lock access
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void lock_data_for_access(CONTROLLER_STAT_T* controller_stat){
	if(sem_down(controller_stat->sem_data_id, MUTEX_DATA_ACCESS)==-1){
		ERROR(M_SEMDOWN_FAILED, "\nError while locking a semaphore");
	}
}

/**
 * @brief release the access for the data
 * @param controller_stat with the reference to the statistical control controller to release new data
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void release_new_data(CONTROLLER_STAT_T* controller_stat){
	int semvalue=0;
	if((semvalue=sem_getvalue(controller_stat->sem_data_id, MUTEX_DATA_AVAILABLE))<0){
		ERROR(M_SEMGETVALUE_FAILED, "\nError while getting a semaphore value");
	}
	if(semvalue==0){
		if(sem_up(controller_stat->sem_data_id, MUTEX_DATA_AVAILABLE)==-1){
			ERROR(M_SEMUP_FAILED, "\nError while releasing a semaphore");
		}
	}
}

/**
 * @brief lock the access to the data
 * @param controller_stat with the reference to the statistical control controller to lock new data
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void wait_for_new_data(CONTROLLER_STAT_T* controller_stat){
	if(sem_down(controller_stat->sem_data_id, MUTEX_DATA_AVAILABLE)==-1){
		ERROR(M_SEMDOWN_FAILED, "\nError while locking a semaphore");
	}
}
