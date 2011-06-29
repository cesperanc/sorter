/**
 * @file aux.h
 * @brief header file for the auxiliary functions
 * @date  2009/10/29 File creation
 * @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
 */

#ifndef AUX_H_
#define AUX_H_

// defines
/**
 * @brief Macro to print on the stderr useful depuration information.
 * It accepts a variable number of parameters
 *
 * @see my_debug() for more information about this function
 */
#define MY_DEBUG(...) my_debug(__FILE__, __LINE__,__VA_ARGS__)

// prototypes
void my_debug(const char*, const int, char*, ...);
int count_file_lines(char*, int);
FILE* open_log_file(char*, char*);
void close_log_file(FILE*);
char* path_filename(char*, char*);
int dir_exists(char*);
int count_dir_items(char*);
int file_exists(char*, char*);
char* get_current_time(char*, int);
float time_diff(struct timeval, struct timeval);
void md5sum(char*, char*);
char* get_line_from_file(char*, int, char*, char*, char*);
char* get_model_name(void);
int ends_with(const char*, const char*);

#endif /* AUX_H_ */
