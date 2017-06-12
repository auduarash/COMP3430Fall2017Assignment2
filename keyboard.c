#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "console.h"
#include "keyboard.h"
#include "player.h"
#include "shared.h"

#define MAX_BUF 1024

extern int player_row;
extern int player_column;

extern bool is_game_over;

void *keyboard_run() {
    fd_set rfds;
    struct timeval tv;
    int retval;


    char buf[MAX_BUF];
    while ( ! is_game_over ) {
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        /*Watch stdin for input*/
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        retval = select(1, &rfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror("select()");
        } else if (retval) {
            read(STDIN_FILENO, buf, 1);
            switch (buf[0]) {
                case 'w': {update_player(-1, 0); break;}
                case 'd': {update_player(0, 1); break;}
                case 'a': {update_player(0, -1); break;}
                case 's': {update_player(1, 0); break;}
            }
            sleepTicks(1);
            // printf("Data is available now. %d %c \n", retval, c);
        } else {
            //TODO: Do some form of quit or message send to main.
            // printf("No data came in within 5 seconds.\n");
        }
    }
    pthread_exit(NULL);
}