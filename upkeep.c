/*

@author: Abdul-Rasheed Audu
@course: COMP 3430 - Operating Systems
@title: single_log.c
@purpose: Handles memory allocated for linked lists. Creates and deletes and 
    reuses linked lists as required.

*/


#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "console.h"
#include "list.h"
#include "upkeep.h"
#include "player.h"
#include "common.h"


/*
    Upkeep helps handle memory for objects that come and go as the program progresses
*/
static LinkedList active_logs;
static LinkedList dead_logs;
static bool upkeep_ready = false;
static pthread_mutex_t active_list_mutex;
static int current_log_id = 0;
static int no_lives = 1;
static int player_score = 0;

extern bool is_game_over;
extern pthread_cond_t game_over;
extern pthread_mutex_t draw_mutex;

void * upkeep_run () {
    pthread_mutex_init(&active_list_mutex, NULL);
    pthread_mutex_lock(&active_list_mutex);
    active_logs = create_linked_list();
    dead_logs = create_linked_list();
    pthread_mutex_unlock(&active_list_mutex);
    upkeep_ready = true;
    update_score();
    while ( ! is_game_over ) {
    }
    pthread_exit(NULL);
}

void clean_up() {
    pthread_mutex_lock(&active_list_mutex);
    clear_linked_list(active_logs);
    clear_linked_list(dead_logs);
    pthread_mutex_unlock(&active_list_mutex);
}

Log get_new_log() {
    while ( ! upkeep_ready ) {
        sleepTicks(10);
    }

    Log new;
    if ( is_empty(dead_logs) ) {
        new = malloc(sizeof(struct LOG));
    } else {
        pthread_mutex_lock(&active_list_mutex);
        new = remove_front(dead_logs);
        pthread_mutex_unlock(&active_list_mutex);
    }
    new->id = current_log_id++;
    pthread_mutex_lock(&active_list_mutex);
    add_item(active_logs, new);
    pthread_mutex_unlock(&active_list_mutex);
    return new;
}


void delete_log(Log expired) {
    bool success = false;
    pthread_mutex_lock(&active_list_mutex);
    expired->is_alive = false;
    success = remove_item(active_logs, expired);

    pthread_mutex_unlock(&active_list_mutex);
    if (success) {
        pthread_mutex_lock(&active_list_mutex);
        add_item(dead_logs, expired);
        join_thread_from_id(expired->thread_id);
        pthread_mutex_unlock(&active_list_mutex);

    }
}

bool place_player_on_log(int row, int column) {
    bool placed = false;
    pthread_mutex_lock(&active_list_mutex);
    Node curr = active_logs->top;
    while (curr != NULL) {
        Log data = curr->data;
        data->player_on_log = false;
        if (data->row == row && column >= data->column_index && column <= data->column_index+22) {
            data->player_on_log = placed = true;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&active_list_mutex);
    return placed;
}

void update_score() {
    pthread_mutex_lock(&draw_mutex);
    char le_lives = no_lives + '0';
    putString(&le_lives, 0, 42, 1);
    pthread_mutex_unlock(&draw_mutex);
}


void live_lost() {
    no_lives -= 1;
    update_score();
    if (no_lives <= 0) {
        pthread_cond_signal(&game_over);
    } else {
        reset_player_position();
    }
}

void frog_crossed_pond() {
    player_score += 1;
    reset_player_position();
}