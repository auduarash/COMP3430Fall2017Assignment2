#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
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

void *draw_screen_run() {
    printf("Starting thread to draw screen. \n");

    if (consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD)) {
        // int i;
        while (true) {

            consoleRefresh();
            sleep(1);
        }
    }
    printf("In loop. Sleeping for 10 seconds\n");
    consoleFinish();


    printf("Done with the draw screen thread. \n");
    return "done draw_screen";
}