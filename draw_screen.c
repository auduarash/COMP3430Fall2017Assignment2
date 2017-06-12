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
extern bool is_game_over;

void *draw_screen_run() {
    pthread_mutex_lock(&draw_mutex);
    if (consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD)) {
        pthread_mutex_unlock(&draw_mutex);
        // int i;
        while (! is_game_over ) {
            pthread_mutex_lock(&draw_mutex);
            consoleRefresh();
            pthread_mutex_unlock(&draw_mutex);
            sleepTicks(1);
        }
    }
    finalKeypress();
    consoleFinish();
    pthread_exit(NULL);;
}