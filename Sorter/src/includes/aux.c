/**
 * @file aux.c
 * @brief source file for the auxiliary functions
 * @date  2009/10/29 File creation
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../3rd/debug.h"
#include "definitions.h"
#include "aux.h"

/**
 * @brief This function is based on the 3rd/debug.c debug function and operates using a very similar mode.
 * However, this function only outputs a debug message if the SHOW_DEBUG is enabled.
 * Unfortunately, by the information that we have found, it isn't possible to call another function with optional parameters without change it.
 * (see http://c-faq.com/varargs/handoff.html for reference)
 *
 * @param file string with the filename that output the debug message
 * @param line integer with the line were this function was called
 * @param format string like the ones used with functions like "printf"
 * @param ... variable number of parameters
 * @see debug
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void my_debug(const char* file, const int line, char* format, ...){
	// silence the warnings
	(void)file; (void)line; (void)format;

	// if we have the SHOW_DEBUG enabled, let's output the error
	#ifdef SHOW_DEBUG
		va_list argp;
		va_start(argp, format);
		fprintf(stderr, "[%s@%d] DEBUG - ", file, line);
		vfprintf(stderr, format, argp);
		va_end(argp);
		fprintf(stderr, "\n");
		fflush(stderr);
	#endif
}

/**
 * @brief Count the number of lines of the given filename
 * @param filename with the filename to count the lines
 * @param maxchars integer with the maximum number of characters to read on each request
 * @return integer with the number of lines, -1 on error
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int count_file_lines(char* filename, int maxchars){
	FILE* file;
	char line[maxchars+1];
	int lines = 0;

	// Open the file
	file = fopen(filename, "r");
	if (file != NULL){
		// Read each line
		while(fgets(line, maxchars, file) != NULL ){
			// Increment the line counter
			lines++;
		}
		// Close the file handler
		fclose (file);
	}else{
		// On error
		lines=-1;
	}
	return lines;
}

/**
 * @brief Open a file descriptor for the log file and redirects the stdout to this file
 * @param filename with the log filename
 * @param attrib attributes to use in the fopen
 * @return FILE* descriptor
 * @see fopen
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
FILE* open_log_file(char* filename, char* attrib){
	FILE *log_file = NULL;

	// Open the file
	if((log_file=fopen(filename,attrib))==NULL){
		return NULL;
	}else{
		// Close the stdout descriptor
		close(fileno(stdout));
		// Put the file descriptor to the position now free position left by closing the stdout
		dup(fileno(log_file));
	}
	return log_file;
}

/**
 * @brief Closes the file descriptor for the log file and restore the stdout behavior
 * @param log_file FILE* with the file descriptor
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void close_log_file(FILE* log_file){
	if(log_file!=NULL){
		// Close the file descriptor
		close(fileno(log_file));
		// Restore the stdout behavior
		dup(fileno(stdout));
		// Close the file descriptor
		fclose(log_file);
		log_file=NULL;
	}
}

/**
 * @brief Concatenates a string with a path and a filename
 * @param path char* with the path
 * @param filename char* with the filename
 * @return char* with the full path
 * @note the return value must be free by the caller
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
char* path_filename(char* path, char* filename){
	char* full_filename=NULL;
	// Allocates the memory for the path/filename string
	if((full_filename=malloc(sizeof(char)*(strlen(path)+strlen(filename)+strlen("/0"))))==NULL){
		ERROR(M_FAILED_MEMORY_ALLOCATION,"\nMemory allocation failed for the absolute filename %s/%s", path, filename);
	}
	// Constructs the path/filename string
	sprintf(full_filename,"%s/%s",path,filename);

	return full_filename;
}

/**
 * @brief Checks if a directory can be opened/exists
 * @param dirname to check
 * @return integer TRUE if the directory exists, FALSE otherwise
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int dir_exists(char* dirname){
	DIR *dir = NULL;
	if((dir = opendir(dirname))!=NULL){
		closedir(dir);
		return TRUE;
	}
	return FALSE;
}

/**
 * @brief Count the number of regular items on a directory
 * @param dirname to check
 * @return integer with the number of items found
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int count_dir_items(char* dirname){
	DIR *dir = NULL;
	struct dirent *dirItem=NULL; 		// to reference a directory item
	struct stat fileDetails;			// to reference the file details
	char* filename=NULL;
	int counter = 0;

	if((dir = opendir(dirname))!=NULL){
		while((dirItem = readdir(dir))!=NULL){
			filename = path_filename(dirname, dirItem->d_name);
			// Read the file attributes
			if(lstat(filename, &fileDetails)==0){
				//if item is a regular file
				if((fileDetails.st_mode & S_IFREG)!=0){
					counter++;
				}
			}
			free(filename); filename=NULL;
		}
		closedir(dir);
	}
	return counter;
}

/**
 * @brief Checks if a file exists in the given mode
 * @param filename with the filename to check
 * @param mode with the mode to use
 * @return integer TRUE if the directory exists, FALSE otherwise
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
int file_exists(char* filename, char* mode){
	FILE *file = NULL;

	// Open the file
	if((file=fopen(filename,mode))!=NULL){
		fclose(file);
		return TRUE;
	}
	return FALSE;
}

/**
 * @brief Get the current time base on the given parameters
 * @param format to format the date
 * @param num_chars with the maximum number of characters of the string
 * @return string with the date in the given format
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
char* get_current_time(char* format, int num_chars){
	char* date = NULL;
	time_t t;
	struct tm *ltm;

	// Let's reserve one more char for the termination of the string
	num_chars++;

	t = time(NULL);
	ltm = localtime(&t);

	if((date=malloc((num_chars)*sizeof(char)))==NULL){
		ERROR(M_FAILED_MEMORY_ALLOCATION,"\nMemory allocation failed for date");
	}

	if (ltm == NULL) {
		ERROR(M_LOCALTIME_FAILED,"\nUnable to get the local time");
	}

	if (strftime(date, num_chars, format, ltm) == 0) {
		ERROR(M_FORMATTIME_FAILED,"\nUnable to format the date");
	}
	return date;
}

/**
 * @brief Calculate the difference between two timeval in seconds
 * @param start the beginning
 * @param end the end
 * @return float with the difference
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
float time_diff(struct timeval start, struct timeval end){
	int seconds_divisor = 1000000;

	return ((float)(((end.tv_sec * seconds_divisor + end.tv_usec) - (start.tv_sec * seconds_divisor + start.tv_usec))))/seconds_divisor;
}

/**
 * @brief Calculate the md5 sum from the given filename and store it on the md5sum_chars string. It uses the md5sum command line utility.
 * @param md5sum_chars string to store the md5 sum
 * @param filename string with the file name to sum
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
void md5sum(char* md5sum_chars, char* filename){
	pid_t pid;
	int descriptors[2], ret, status;

	/* Pipe creation. (to be used by the two processes) */
	ret = pipe(descriptors);
	if(ret==1){
		ERROR(M_PIPECREATION_FAILED, "Unable to create the pipe\n");
	}

	switch (pid = fork()) {
		case -1: /* On fork error */
			ERROR(M_FORK_FAILED, "Error creating the fork");
			break;
		case 0: /* Child code */
			// close the entry descriptor
			close(descriptors[0]);

			// close the stdout to be available for our descriptor
			close(fileno(stdout));
			close(fileno(stderr));

			// put our descriptor in the first available position on the pipeline (from the stdout that we've released in the previous instruction)
			dup(descriptors[1]);

			// let's call the md5sum program
			execlp("md5sum", "md5sum", filename, NULL);

			ERROR(M_PROGRAMEXECUTION_FAILED, "Error while executing the md5sum command\n");

			break;
		default: /* parent */
			// close the output descriptor
			close(descriptors[1]);

			// wait for the termination of the child execution
			waitpid(pid, &status, 0);

			// check for the exit status of the child
			if(WIFEXITED(status)){
				switch(status){
					case 0:
						// read the md5sum program output stored on the descriptor
						ret = read(descriptors[0], md5sum_chars, sizeof(char)*(MD5SUMCHARS));
						if(ret==1){
							ERROR(M_READINGFROMPIPE_FAILED,"Error reading from pipe");
						}
						// to end the string
						md5sum_chars[MD5SUMCHARS]='\0';

						break;

					default:
						ERROR(M_INVALID_OUTPUT, "Invalid output\n");
				}
			}else{
				ERROR(M_ABNORMAL_TERMINATION, "Abnormal termination\n");
			}
			// close the descriptor
			close(descriptors[0]);
	}
}

/**
 * @brief Get the value of a configuration line from a file
 * @param filename to read the line from
 * @param maxchars maximum number of characters to read on each line
 * @param label from the line
 * @param separators array of separator characters that separate the label from it value
 * @param spacers arrays of spacer characters between the label and the separator, and the separator and the value
 * @return string with the value read
 * @note the return string must be free'd manually after no longer necessary
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
char* get_line_from_file(char* filename, int maxchars, char* label, char* separators, char* spacers){
	FILE* file;
	char line[maxchars+1];
	char* substring=NULL;
	unsigned int line_index, brk, sub_string_size;
	char break_lines[3] = "\n\r";

	maxchars++; // add space for the null character

	// Open the file
	file = fopen(filename, "r");
	if (file != NULL){
		// Read each line
		while(fgets(line, maxchars, file) != NULL ){
			line[maxchars]='\0'; // Terminate the string
			// If we can find a line with that label
			if((substring = strstr(line, label))!=NULL){
				// Verify if the substring has the same pointer than line (we want to match the beginning of the line and not any other position in the line)
				if(substring==line){
					// Remove all the break lines and end the string on that spot
					for(line_index=0; line_index<strlen(line); line_index++){
						if(index(break_lines,line[line_index])!=NULL){
							line[line_index]='\0';
						}
					}
					// Parse the line, char by char, until finding the data for the label
					for(line_index=(strlen(label)+1), brk=FALSE; line_index<strlen(line); line_index++){
						if(index(separators,line[line_index])!=NULL){ // the current char is a separator?
							brk=TRUE; // signal to stop on the next no space or no separator char
							continue; // we want to continue until we find a char that isn't a space or separator
						}else if(index(spacers,line[line_index])!=NULL){ // the current char is a spacer?
							continue;
						}else if(brk==TRUE){
							// the data begins were
							sub_string_size = (strlen(line)-line_index);
							if((substring = malloc(sizeof(char)*(sub_string_size+1)))!=NULL){
								// Let's substring the string
								strncpy(substring, line+line_index, sizeof(char)*sub_string_size);
								substring[sub_string_size]='\0';
								fclose (file);
								return substring;
							}else{
								fclose (file);
								ERROR(M_FAILED_MEMORY_ALLOCATION, "Error in memory allocation");
							}
						}
						break;
					}
				}
			}
		}
		// Close the file handler
		fclose (file);
	}
	return NULL;
}

/**
 * @brief Return the model name from the /proc/cpuinfo file
 * @return string with the model name value
 * @see get_line_from_file for more info
 *
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */
char* get_model_name(void){
	return get_line_from_file("/proc/cpuinfo", MAXCHARS, "model name", ":", " ");
}

/**
 * @brief Verify if a string ends with another string
 * @param haystack string that should end with needle
 * @param needle string that haystack should end with
 * @return integer TRUE if the haystack ends with the needle, FALSE otherwise
 *
 * @author gnud http://stackoverflow.com/questions/1711095/parse-out-the-file-extension-from-a-file-path-in-c
 */
int ends_with(const char* haystack, const char* needle){
    size_t hlen;
    size_t nlen;
    hlen = strlen(haystack);
    nlen = strlen(needle);
    // If the haystack can contain the needle and the haystack ends with needle
    if((nlen <= hlen) && ((strcasecmp(&haystack[hlen-nlen], needle)) == 0)){
    	return TRUE;
    }
    return FALSE;
}
