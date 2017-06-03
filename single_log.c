#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include "draw_screen.h"
#include "list.h"
#include "console.h"
#include "upkeep.h"
#include "single_log.h"
#include "upkeep.h"
#include "player.h"

#define LOG_OFFSET 5
#define LOG_LENGTH 24

static char *top = "/======================\\";
static char *bottom = "\\======================/";


extern pthread_mutex_t draw_mutex;

/*
    move_log - moves the log using appropriate parameters
        move_log
        if player_on_log
            move_player in direction of log
        
        if log_finally_left
            delete_log using some method somewhere
*/

void move_log( Log log ) {
    int old_column = log->column_index;
    int top_bar = log->row_index;
    int bottom_bar = top_bar + 3;
    if (log->player_on_log) {
        update_player(0, log->direction);
    }
    pthread_mutex_lock(&draw_mutex);
    
    //clear the top bar
    consoleClearImage(top_bar, old_column, 1, LOG_LENGTH);
    consoleDrawImage(top_bar, old_column+log->direction, &top, 1);
    
    //clear the bottom bar
    consoleClearImage(bottom_bar, old_column, 1, LOG_LENGTH);
    consoleDrawImage(bottom_bar, old_column+log->direction, &bottom, 1);

    pthread_mutex_unlock(&draw_mutex);
    log->column_index += log->direction;
    
    if (log->direction < 0 && log->column_index < - LOG_LENGTH  ) {
        delete_log(log);
        pthread_exit(NULL);
    } else if (log->direction > 0 && log->column_index >= GAME_COLS) {
        delete_log(log);
        pthread_exit(NULL);
    }
    sleepTicks(log->frequency);
}

void set_new_log_params(Log log, int row, int direction ) {
    log->row = row;
    log->column_index = (direction < 0) ? GAME_COLS : -LOG_LENGTH;
    log->row_index = 4 * row + 4;
    log->player_on_log = false;
    log->direction = direction;
    log->frequency = 20 / (row + 1);
}


void * single_log_run( void * args ) {
    SingleLogArgs log_params = (SingleLogArgs) args;
    Log log = get_new_log();
    set_new_log_params(log, log_params->row, log_params->direction);

    while ( true ) {
        move_log(log);
    }
    return NULL;
}