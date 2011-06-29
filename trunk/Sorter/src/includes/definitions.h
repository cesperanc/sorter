/**
* @file definitions.h
* @brief Header file for the common definitions
* @date 2009/10/31 File creation
* @author Cláudio Esperança <cesperanc@gmail.com>, Diogo Serra <2081008@student.estg.ipleiria.pt>
*/

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

/**
 * A default type for a boolean false value
 */
#define FALSE 0

/**
 * A default type for a boolean true value
 */
#define TRUE 1

/**
 * Constant with the maximum number of chars that a line can contain
 */
#define MAXCHARS 1024

/**
 * Constant with the number of chars of a md5 sum
 */
#define MD5SUMCHARS 32

/**
 * The minimum value for a port
 */
#define PORT_RANGE_MIN 1

/**
 * The maximum value for a port
 */
#define PORT_RANGE_MAX 65535

/**
 * The nickname for the results server
 */
#define NICKNAME "antiThreads"

/**
 * The global definition for the "-ERR Invalid format" server error message
 */
#define RESULTS_SERVER_INVALID_FORMAT "-ERR Invalid format"

/**
 * The global definition for the "-ERR Invalid file" server error message
 */
#define RESULTS_SERVER_INVALID_FILE "-ERR Invalid file"

/**
 * The global definition for the "-ERR Invalid checksum" server error message
 */
#define RESULTS_SERVER_INVALID_CHECKSUM "-ERR Invalid checksum"

/**
 * The global definition for the "-ERR Invalid time" server error message
 */
#define RESULTS_SERVER_INVALID_TIME "-ERR Invalid time"

/**
 * The global definition for the "+OK" server message
 */
#define RESULTS_SERVER_OK "+OK"

/**
 * The global definition for the show_stats file export extension
 */
#define SHOW_STATS_FILE_EXPORT_EXTENSION ".csv"

/**
 * The global definition for the maximum connections number for the HTTP show_stats server
 */
#define HTTP_MAXIMUM_CONNECTIONS 10

// mutexes
/**
 * Mutex constant for the statistical control semaphore
 */
#define MUTEX_CONTROL_STATS 0

/**
 * Mutex constant for the statistical control semaphore
 */
#define MUTEX_EXIT 1

/**
 * Mutex constant for the data available semaphore
 */
#define MUTEX_DATA_AVAILABLE 0

/**
 * Mutex constant for the data ACCESS semaphore
 */
#define MUTEX_DATA_ACCESS 1

// exit messages
/**
 * Define the exit value for the invalid parameters message
 */
#define M_INVALID_PARAMETERS 1

/**
 * Define the exit value for the failed memory allocation message
 */
#define M_FAILED_MEMORY_ALLOCATION 2

/**
 * Define the exit value for the failed file read message
 */
#define M_FAILED_FILE_READ 3

/**
 * Define the exit value for the failed to retrieve the file lines message
 */
#define M_FAILED_RETRIEVE_FILE_LINES_COUNT 4

/**
 * Define the exit value for the file bigger than expected message
 */
#define M_FILE_BIGGER_THAN_EXPECTED 5

/**
 * Define the exit value for the processing failed message
 */
#define M_PROCESSING_FAILED 6

/**
 * Define the exit value for the open dir failed message
 */
#define M_OPEN_DIR_FAILED 7

/**
 * Define the exit value for the error reading file attributes message
 */
#define M_FILE_ATTRIBUTES_READ_FAILED 8

/**
 * Define the exit value for the error creating a clone message
 */
#define M_CLONE_CREATION_FAILED 9

/**
 * Define the exit value for the error creating the output file message
 */
#define M_FILE_OUTPUT_FAILED 10

/**
 * Define the exit value for the error sort message
 */
#define M_SORT_FAILED 10

/**
 * Define the exit value for the error reading the localtime message
 */
#define M_LOCALTIME_FAILED 11

/**
 * Define the exit value for the error reading the localtime message
 */
#define M_FORMATTIME_FAILED 12

/**
 * Define the exit value for the error reading the hostname
 */
#define M_GETHOSTNAME_FAILED 13

/**
 * Define the exit value for the error missing log in the daemon mode
 */
#define M_DAEMON_BUT_NO_LOG_FILE 14

/**
 * Define the exit value for the fork failed message
 */
#define M_FORK_FAILED 15

/**
 * Define the exit value for the error creating a pipe
 */
#define M_PIPECREATION_FAILED 16

/**
 * Define the exit value for the error executing the program
 */
#define M_PROGRAMEXECUTION_FAILED 17

/**
 * Define the exit value for the error reading from pipe
 */
#define M_READINGFROMPIPE_FAILED 18

/**
 * Define the exit value for the invalid output error
 */
#define M_INVALID_OUTPUT 19

/**
 * Define the exit value for the abnormal termination error
 */
#define M_ABNORMAL_TERMINATION 20

/**
 * Define the exit value for the sigaction sigint error
 */
#define M_SIGACTION_SIGINT_FAILED 21

/**
 * Define the exit value for the unknown algorithm error
 */
#define M_UNKNOWN_ALGORITHM 22

/**
 * Define the exit value for the ftok error
 */
#define M_FTOK_FAILED_FOR_DATA 23

/**
 * Define the exit value for the shared memory allocation error
 */
#define M_SHMGET_FAILED_FOR_DATA 24

/**
 * Define the exit value for the semaphore creation error
 */
#define M_SEMCREATE_FAILED_FOR_DATA 25

/**
 * Define the exit value for the shared memory attach error
 */
#define M_SHMAT_FAILED_FOR_DATA_STATS 26

/**
 * Define the exit value for the shared memory detach error
 */
#define M_SHMDT_FAILED 27

/**
 * Define the exit value for the shared memory control error
 */
#define M_SHMCTL_FAILED 28

/**
 * Define the exit value for the semaphore deletion error
 */
#define M_SEMDELETE_FAILED 29

/**
 * Define the exit value on error while getting a semaphore
 */
#define M_SEMGET_FAILED_FOR_DATA 30

/**
 * Define the exit value for the locked control data resource
 */
#define M_LOCKED_CONTROL_STATS 31

/**
 * Define the exit value on error while locking a semaphore
 */
#define M_SEMDOWN_FAILED 32

/**
 * Define the exit value on error while releasing a semaphore
 */
#define M_SEMUP_FAILED 33

/**
 * Define the exit value on error while setting the value of a semaphore
 */
#define M_SEMSET_FAILED_FOR_DATA 34

/**
 * Define the exit value for the maximum shared memory attach for the statistical controller
 */
#define M_MAXIMUM_SHMAT_CONTROL_STATS 35

/**
 * Define the exit value for the ftok error for the control key
 */
#define M_FTOK_FAILED_FOR_CONTROL_KEY 36

/**
 * Define the exit value for the shared memory allocation error for the controller
 */
#define M_SHMGET_FAILED_FOR_CONTROL_ID 37

/**
 * Define the exit value for the semaphore creation error for the controller
 */
#define M_SEMCREATE_FAILED_FOR_CONTROL 38

/**
 * Define the exit value on error while setting the value of a control semaphore
 */
#define M_SEMSET_FAILED_FOR_CONTROL 39

/**
 * Define the exit value for the shared memory attach error for the control
 */
#define M_SHMAT_FAILED_FOR_CONTROL 40

/**
 * Define the exit value on error while getting a semaphore for the control
 */
#define M_SEMGET_FAILED_FOR_CONTROL 41

/**
 * Define the exit value when there is no program attached to the shared memory segment
 */
#define M_NO_PROGRAM_ATTACHED_TO_MEMORY 42

/**
 * Define the exit value on error while getting the value of a semaphore
 */
#define M_SEMGETVALUE_FAILED 43

/**
 * Define the exit value for the error in the number conversion message
 */
#define M_NUMBER_CONVERSION_ERROR 44

/**
 * Define the exit value for the no digits found message
 */
#define M_NO_DIGITS_FOUND 45

/**
 * Define the exit value for the port out of range message
 */
#define M_PORT_OUT_OF_RANGE 46

/**
 * Define the exit value for the socket creation error
 */
#define M_SOCKET_CREATION_ERROR 47

/**
 * Define the exit value for the bind socket error
 */
#define M_BIND_SOCKET_ERROR 48

/**
 * Define the exit value for the receive from error
 */
#define M_RECVFROM_ERROR 49

/**
 * Define the exit value for the send to error
 */
#define M_SENDTO_ERROR 50

/**
 * Define the exit value for the invalid IP address error
 */
#define M_INVALID_IP_ADDRESS 51

/**
 * Define the exit value for the unknown IP address error
 */
#define M_UNKNOWN_IP_ADDRESS 52

/**
 * Define the exit value for the retrieve model name error
 */
#define M_RETRIEVE_MODEL_NAME_ERROR 53

/**
 * Define the exit value for the getaddrinfo error
 */
#define M_GETADDRINFO_ERROR 54

/**
 * Define the exit value for the setsockopt error
 */
#define M_SETSOCKOPT_ERROR 55

/**
 * Define the exit value for the bind server error
 */
#define M_BIND_SERVER_ERROR 56

/**
 * Define the exit value for the socket listen error
 */
#define M_SOCKET_LISTEN_ERROR 57

/**
 * Define the exit value for the sigaction sigchld error
 */
#define M_SIGACTION_SIGCHLD_FAILED 58

/**
 * Define the exit value for the pthread_mutex_init error
 */
#define M_PTHREAD_MUTEX_INIT_FAILED 59

/**
 * Define the exit value for the pthread_create error
 */
#define M_PTHREAD_CREATE_FAILED 60

/**
 * Define the exit value for the minimum number of files error
 */
#define M_NUMBER_OF_FILES 61

#endif /* DEFINITIONS_H_ */
