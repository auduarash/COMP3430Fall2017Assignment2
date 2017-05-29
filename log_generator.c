#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "log_generator.h"
#include "console.h"

static char *LOG_GRAPHIC[4] = {
        "/======================\\",
        "",
        "",
        "\\======================/"
};
extern pthread_mutex_t draw_mutex;
extern pthread_cond_t wait_for_console;


void *log_generator_run(void *arg) {
    //Do not start until we receive a signal
    int index = *((int *) arg);

    printf("I am a log generator %d \n", index);
    char **to_draw = LOG_GRAPHIC;

    pthread_mutex_lock(&draw_mutex);
    consoleDrawImage(4 + index * 4, 0, to_draw, 4);
    pthread_mutex_unlock(&draw_mutex);

    return NULL;
}