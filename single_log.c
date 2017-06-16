/*

@author: Abdul-Rasheed Audu
@course: COMP 3430 - Operating Systems
@title: single_log.c
@purpose: Logic for the movement
    and animation of a single log moving across the screen

*/

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
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
static char *animations[][4] = {
    {"|", "|", "|", "|"},
    {"|", "-", "|", "+"},
    {"-", "|", "+", "|"},
    {"-", "-", "+", "+"}
};
static int top_x_animation[] = {4, 8, 12, 16};
static int bottom_x_animation[] = {1, 13, 8, 20};
static char *x_animation = "X";


extern pthread_mutex_t draw_mutex;
extern bool is_game_over;

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

    consoleClearImage(top_bar+1, old_column, 1, 1);
    consoleDrawImage(top_bar+1, old_column+log->direction, &animations[log->animation][0], 1);
    
    consoleClearImage(top_bar+1, old_column + LOG_LENGTH - 1, 1, 1);
    consoleDrawImage(top_bar+1, old_column+log->direction + LOG_LENGTH - 1, &animations[log->animation][2], 1);
    
    consoleClearImage(top_bar+2, old_column, 1, 1);
    consoleDrawImage(top_bar+2, old_column+log->direction, &animations[log->animation][1], 1);
    
    consoleClearImage(top_bar+2, old_column + LOG_LENGTH-1, 1, 1);
    consoleDrawImage(top_bar+2, old_column+log->direction+LOG_LENGTH-1, &animations[log->animation][3], 1);

    consoleClearImage(top_bar+1, old_column + top_x_animation[log->prev_animation], 1, 1);
    consoleDrawImage(top_bar+1, old_column + log->direction + top_x_animation[log->animation], &x_animation, 1);
    
    consoleClearImage(top_bar+2, old_column + bottom_x_animation[log->prev_animation], 1, 1);
    consoleDrawImage(top_bar+2, old_column + log->direction + bottom_x_animation[log->animation], &x_animation, 1);
    log->prev_animation = log->animation;
    

    pthread_mutex_unlock(&draw_mutex);
    log->column_index += log->direction;
    
    if (log->direction < 0 && log->column_index < - LOG_LENGTH  ) {
        delete_log(log);
    } else if (log->direction > 0 && log->column_index >= GAME_COLS) {
        delete_log(log);
    }
}

void set_new_log_params(Log log, int row, int direction ) {
    log->is_alive = true;
    log->row = row;
    log->column_index = (direction < 0) ? GAME_COLS : -LOG_LENGTH;
    log->row_index = 4 * row + 4;
    log->player_on_log = false;
    log->direction = direction;
    log->frequency = 10 / (5 - row);
    log->animation = log->prev_animation = 0;

}

void *update_thread_anim(void *arg) {
    Log log = (Log) arg;
    while ( log->is_alive ) {
        pthread_mutex_lock(&draw_mutex);
        log->animation = (log->animation + 1 ) % 4;
        pthread_mutex_unlock(&draw_mutex);
        sleepTicks(50);
    }
    pthread_exit(NULL);
}

void * single_log_run( void * args ) {
    SingleLogArgs log_params = (SingleLogArgs) args;
    Log log = get_new_log();
    set_new_log_params(log, log_params->row, log_params->direction);

    thread_ptr update_thread = create_thread_object(update_thread_anim, log);
    while ( ! is_game_over && log->is_alive ) {
        move_log(log);
        sleepTicks(log->frequency);
    }
    join_thread(update_thread);
    free(update_thread);
    pthread_exit(NULL);
}