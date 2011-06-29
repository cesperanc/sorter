/**
* @file showstatslib.h
* @brief header file for the showStats functions
* @date 2009/12/12 File creation
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#ifndef SHOWSTATSLIB_H_
#define SHOWSTATSLIB_H_

int initializes_controller_stat(CONTROLLER_STAT_T* , char*);
int remove_controller_stat(CONTROLLER_STAT_T*);
void add_listener(CONTROLLER_STAT_T*);
void remove_listener(CONTROLLER_STAT_T*);
int parse_initializes_controller_stat_result(int, CONTROLLER_STAT_T*);

#endif /* SHOWSTATSLIB_H_ */
