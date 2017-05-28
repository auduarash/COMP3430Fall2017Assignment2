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

void *draw_screen_run() {
    printf("Starting thread to draw screen. \n");

    if (consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD)) {
        // int i;
        while (true) {
            pthread_mutex_lock(&draw_mutex);
            consoleRefresh();
            pthread_mutex_unlock(&draw_mutex);
            sleepTicks(1);
        }
    }
    printf("In loop. Sleeping for 10 seconds\n");
    consoleFinish();


    printf("Done with the draw screen thread. \n");
    return "done draw_screen";
}