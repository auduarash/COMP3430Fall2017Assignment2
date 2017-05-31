#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "common.h"
#include "single_log.h"
#include "log_generator.h"
#include "console.h"

static char *LOG_GRAPHIC[4] = {
        "/======================\\",
        "",
        "",
        "\\======================/"
};
extern bool console_ready;
extern pthread_mutex_t draw_mutex;
extern pthread_cond_t wait_for_console;

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
    thread_ptr log_thread = create_thread_object(1, "Log thread", single_log_run, NULL);
    int s;

    //Do not start until console has been initialized
    while ( ! console_ready );
    s = pthread_join(log_thread->thread_id, NULL);
    validate_call(s, "pthread_join");

    pthread_exit(NULL);

    return NULL;
}