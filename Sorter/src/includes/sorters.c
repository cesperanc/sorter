/**
* @file sorters.c
* @brief Main source file for the Sorters programs
* @date 2009/10/31 File creation
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../3rd/debug.h"
#include "definitions.h"
#include "commonlib.h"
#include "sorterlib.h"
#include "sorters.h"


/**
 * @brief Sort the FILE_LINES_T using the merge sort algorithm
 * @param flines FILE_LINES_T with the lines to sort
 * @param stat ALGORITHM_STAT_T with the structure to store the statistical data of the sort operation
 * @return FILE_LINES_T with the sorted lines
 */
FILE_LINES_T* merge_sort(FILE_LINES_T* flines, ALGORITHM_STAT_T* stat) {
	FILE_LINES_T* flines_clone = NULL;

	stat->nlines = flines->num_lines;

	if((flines_clone = clone_of_lines(flines))==NULL){
		ERROR(M_CLONE_CREATION_FAILED, "\nError creation a copy of the lines object");
	}

	merge_sort_aux((char**)flines->lines,(char**)flines_clone->lines, flines->num_lines, stat);
	free_memory_of_clone_of_lines(flines_clone);

	return flines;
}

/**
 * @brief Auxiliary function with the merge sort algorithm
 * @param **flines_src with the lines to sort
 * @param **aux with with the lines clone to use as auxiliary object
 * @param size integer with the number of lines to process
 * @param stat ALGORITHM_STAT_T with the structure to store the statistical data of the sort operation
 *
 * @see http://www.codecodex.com/wiki/Merge_sort#C
 * @see https://www-927.ibm.com/ibm/cas/hspc/MergeSort1.shtml
 * @see http://en.literateprograms.org/Merge_sort_(C)
 */
void merge_sort_aux(char** flines_src, char** aux, int size, ALGORITHM_STAT_T* stat){
    int i1, i2, tempi;

    // If we can't divide anymore, this is the end
    if (size <= 1) {
		return;
	}

    merge_sort_aux(flines_src, aux, size/2, stat);
    merge_sort_aux(flines_src + size/2, aux, size - size/2, stat);

    i1 = 0;
    i2 = size/2;
    tempi = 0;
    while (i1 < size/2 && i2 < size) {
		stat->niterations++;
        if (strcmp(flines_src[i1],flines_src[i2])<0) {
        	aux[tempi] = flines_src[i1];
            i1++;
        } else {
        	aux[tempi] = flines_src[i2];
            i2++;
        }
        tempi++;
    }
    // First half
    while (i1 < size/2) {
		stat->nswaps++;
    	aux[tempi] = flines_src[i1];
        i1++;
        tempi++;
    }
    // Second half
    while (i2 < size) {
		stat->nswaps++;
    	aux[tempi] = flines_src[i2];
        i2++;
        tempi++;
    }

    // Copy from the clone to the sorted object
    for(tempi=0; tempi<size; tempi++){
    	flines_src[tempi] = aux[tempi];
    }
}

/**
 * @brief Sort the FILE_LINES_T using the shell sort algorithm
 * @param flines FILE_LINES_T with the lines to sort
 * @param stat ALGORITHM_STAT_T with the structure to store the statistical data of the sort operation
 * @return FILE_LINES_T with the sorted lines
 *
 * @see http://pt.wikipedia.org/wiki/Shell_sort#C.C3.B3digo_em_C
 */
FILE_LINES_T* shell_sort(FILE_LINES_T* flines, ALGORITHM_STAT_T* stat) {
	int i, j, gap = 1;
	char* aux;

	stat->nlines = flines->num_lines;

	do{
		gap = 3*gap+1;
	}while(gap < flines->num_lines);

	do{
		gap /= 3;
		for(i=gap; i<flines->num_lines; i++) {
			stat->niterations++;
			aux=flines->lines[i];
			j=i-gap;

			while (j>=0 && strcmp(aux,flines->lines[j])<0) {
				stat->nswaps++;
				flines->lines[j+gap] = flines->lines[j];
                j-=gap;
            }
			flines->lines[j+gap] = aux;
        }
    } while ( gap > 1);

    return flines;
}

/**
 * @brief Sort the FILE_LINES_T using the quick sort algorithm
 * @param flines FILE_LINES_T with the lines to sort
 * @param stat ALGORITHM_STAT_T with the structure to store the statistical data of the sort operation
 * @return FILE_LINES_T with the sorted lines
 */
FILE_LINES_T* quick_sort(FILE_LINES_T* flines, ALGORITHM_STAT_T* stat) {
	stat->nlines = flines->num_lines;
	return quick_sort_aux(flines, 0, flines->num_lines-1, stat);
}

/**
 * @brief Sort the FILE_LINES_T using the quick sort algorithm
 * @param flines FILE_LINES_T with the lines to sort
 * @param begin integer
 * @param end integer
 * @param stat ALGORITHM_STAT_T with the structure to store the statistical data of the sort operation
 * @return FILE_LINES_T with the sorted lines
 *
 * @see http://www.codecodex.com/wiki/Quicksort#C
 */
FILE_LINES_T* quick_sort_aux(FILE_LINES_T* flines, int begin, int end, ALGORITHM_STAT_T* stat){
	char* piv; char* tmp;
	int  l,r,p;

	while (begin<end){    // This while loop will avoid the second recursive call
		stat->niterations++;
		l = begin; p = (begin+end)/2; r = end;
		piv = flines->lines[p];
		while (1){
			while ( (l<=r) && ( strcmp(flines->lines[l],piv) <= 0 ) ) l++;
			while ( (l<=r) && ( strcmp(flines->lines[r],piv)  > 0 ) ) r--;
			if (l>r) break;
			tmp=flines->lines[l]; flines->lines[l]=flines->lines[r]; flines->lines[r]=tmp;
			stat->nswaps++;
			if (p==r) p=l;
			l++; r--;
		}
		flines->lines[p]=flines->lines[r]; flines->lines[r]=piv;
		r--;
		// Recursion on the shorter side & loop (with new indexes) on the longer
		if ((r-begin)<(end-l)){
			quick_sort_aux(flines, begin, r, stat);
			begin=l;
		}else{
			quick_sort_aux(flines, l, end, stat);
            end=r;
        }
    }
	return flines;
}

/**
 * @brief Sort the FILE_LINES_T using the bubble sort algorithm
 * @param flines FILE_LINES_T with the lines to sort
 * @param stat ALGORITHM_STAT_T with the structure to store the statistical data of the sort operation
 * @return FILE_LINES_T with the sorted lines
 *
 * @see http://pt.wikipedia.org/wiki/Bubble_sort#C_2
 */
FILE_LINES_T* bubble_sort(FILE_LINES_T* flines, ALGORITHM_STAT_T* stat){
	int i, j, test;
	char* aux;

	stat->nlines = flines->num_lines;

	for(i = flines->num_lines - 1; i > 0; i--){
		test=0;
		for(j = 0; j < i; j++){
			stat->niterations++;
			if(strcmp(flines->lines[j], flines->lines[j+1])>0){
				stat->nswaps++;
				aux = flines->lines[j];    //swap array[j] and array[j+1]
				flines->lines[j] = flines->lines[j+1];
				flines->lines[j+1] = aux;
				test=1;
			}
		} //end for j
		if(test==0)
			break; //will exit if the list is sorted!
	} //end for i
	return flines;
}

/**
 * @brief Sort the FILE_LINES_T using the bubble sort algorithm
 * @param flines FILE_LINES_T with the lines to sort
 * @param stat ALGORITHM_STAT_T with the structure to store the statistical data of the sort operation
 * @return FILE_LINES_T with the sorted lines
 */
FILE_LINES_T* q_sort(FILE_LINES_T* flines, ALGORITHM_STAT_T* stat){
	stat->nlines = flines->num_lines;

	qsort(flines->lines, flines->num_lines, sizeof(char*), q_sort_aux);
	return flines;
}

/**
 * @brief Auxiliary function for q_sort algorithm
 * @param a string to be compared with the next parameter
 * @param b string to be compared with the previous parameter
 * @return integer 0 if the strings are equal, 1 or -1 otherwise
 */
int q_sort_aux(const void *a, const void *b){
	const char **ia = (const char **)a;
	const char **ib = (const char **)b;

	return strcmp(*ia, *ib);
}
