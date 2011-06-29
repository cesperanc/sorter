/**
* @file sorters.h
* @brief sorters program
* @date 2009/10/31 File creation
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#ifndef SORTERS_H_
#define SORTERS_H_
FILE_LINES_T* bubble_sort(FILE_LINES_T*, ALGORITHM_STAT_T*);
FILE_LINES_T* quick_sort(FILE_LINES_T*, ALGORITHM_STAT_T*);
FILE_LINES_T* quick_sort_aux(FILE_LINES_T* , int, int, ALGORITHM_STAT_T*);
FILE_LINES_T* shell_sort(FILE_LINES_T*, ALGORITHM_STAT_T*);
FILE_LINES_T* merge_sort(FILE_LINES_T*, ALGORITHM_STAT_T*);
void merge_sort_aux(char**, char**, int, ALGORITHM_STAT_T*);
int q_sort_aux(const void *, const void *);
FILE_LINES_T* q_sort(FILE_LINES_T*, ALGORITHM_STAT_T*);

#endif /* SORTERS_H_ */
