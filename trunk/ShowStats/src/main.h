#ifndef __MAIN_H
#define __MAIN_H

void show_stats(CONTROLLER_STAT_T*, WEB_SERVER_PARAMS_T *);
FILE* csvnize(struct gengetopt_args_info);
void webnize(struct gengetopt_args_info, int, pthread_t *, WEB_SERVER_PARAMS_T *);
void print_header(CONTROLLER_STAT_T);
void register_show_stats(CONTROLLER_STAT_T*);

#endif

