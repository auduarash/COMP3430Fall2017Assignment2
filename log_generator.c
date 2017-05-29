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
extern bool console_ready;
extern pthread_mutex_t draw_mutex;
extern pthread_cond_t wait_for_console;


void *log_generator_run(void *arg) {
    int index = *((int *) arg);

    // TODO: Fix up this bad boy
    while ( true ) {

        
        sleepTicks(25);
    }
    char **to_draw = LOG_GRAPHIC;

    //Do not start until console has been initialized
    while ( ! console_ready );
    pthread_mutex_lock(&draw_mutex);
    consoleDrawImage(4 + index * 4, 0, to_draw, 4);
    pthread_mutex_unlock(&draw_mutex);

    return NULL;
}