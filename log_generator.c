#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "common.h"
#include "single_log.h"
#include "log_generator.h"
#include "console.h"

extern pthread_mutex_t draw_mutex;
extern pthread_cond_t wait_for_console;

extern bool is_game_over;

LogGeneratorParam create_log_generator_params(int index) {
    LogGeneratorParam params = malloc(sizeof(struct LOG_GENERATOR_PARAMS));
    if (params != NULL ){
        params->index = index;
    }
    return params;
}



void *log_generator_run(void *arg) {
    LogGeneratorParam param = (LogGeneratorParam) arg;
    if (param == NULL) {
        //TODO: Exit the thread here
    }
    int index = param->index;
    SingleLogArgs s_args = malloc(sizeof(struct SINGLE_LOG_ARGS));
    s_args->row = index;
    s_args->direction = (index % 2) ? 1 : -1;
    s_args->refresh_ticks = 25;



    while ( ! is_game_over ) {
        create_thread_object(1, "Log thread", single_log_run, s_args);
        sleepTicks(200);
    }

    pthread_exit(NULL);
        

    return NULL;
}