#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "list.h"
#include "console.h"
#include "single_log.h"

#define LOG_OFFSET 5

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
    int old_x = log->x_location;
    int top_bar = log->y_location;
    int bottom_bar = top_bar + 3;
    pthread_mutex_lock(&draw_mutex);
    
    //clear the top bar
    consoleClearImage(top_bar, old_x, 1, strlen(top));
    consoleDrawImage(top_bar, old_x+log->direction, &top, 1);
    
    //clear the bottom bar
    consoleClearImage(bottom_bar, old_x, 1, strlen(bottom));
    consoleDrawImage(bottom_bar, old_x+log->direction, &bottom, 1);
    pthread_mutex_unlock(&draw_mutex);
    log->x_location += log->direction;
    sleepTicks(log->frequency);
}


void * single_log_run( void * args ) {
    SingleLogArgs log_params = (SingleLogArgs) args;
    Log log = malloc(sizeof(struct LOG));

    //TODO: Change this to a random value or something based on direction
    log->x_location = 0; 
    log->y_location = 4 * log_params->row + 4;
    log->player_on_log = false;
    log->direction = (log_params->row % 2) ? 1 : -1;
    log->frequency = 20 / (log_params->row + 1);
    log->animation = 0;


    while ( true ) {
        move_log(log);
    }
    return NULL;
}