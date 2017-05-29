#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include "console.h"
#include "draw_screen.h"

static char *GAME_BOARD[] = {
"                                   Lives:",
"/------\\          /------\\          /------\\          /------\\          /------\\",
"|      |          |      |          |      |          |      |          |      |",
"+      +----------+      +----------+      +----------+      +----------+      +",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"",
"",
"",
"" };
extern pthread_mutex_t draw_mutex;
extern pthread_cond_t wait_for_console;
extern bool console_ready;

void *draw_screen_run() {
    printf("Starting thread to draw screen. \n");
    printf("Starting console\n");
    pthread_mutex_lock(&draw_mutex);
    if (consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD)) {
        console_ready = true;
        pthread_mutex_unlock(&draw_mutex);
        pthread_cond_broadcast(&wait_for_console);
        // int i;
        while (true) {
            pthread_mutex_lock(&draw_mutex);
            consoleRefresh();
            pthread_mutex_unlock(&draw_mutex);
            sleepTicks(1);
        }
    } else {
        //We could not init the console. Do something about it

    }
    consoleFinish();


    printf("Done with the draw screen thread. \n");
    return "done draw_screen";
}