#ifndef __MAIN_H
#define __MAIN_H

void print_log_header(struct gengetopt_args_info, int, char **);
int processDir(struct gengetopt_args_info, int, char **);
void handle_signal(int);
void register_signal_handlers(void);
void daemonize(struct gengetopt_args_info);
FILE* lognize(struct gengetopt_args_info);
void remove_duplicated_algorithms(struct gengetopt_args_info*);

#endif

