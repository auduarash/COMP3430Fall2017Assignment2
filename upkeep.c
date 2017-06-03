#include <stdlib.h>
#include <pthread.h>
#include "console.h"
#include "list.h"
#include "upkeep.h"

/*
    Upkeep helps handle memory for objects that come and go as the program progresses
*/
static LinkedList active_logs;
static LinkedList dead_logs;
static bool upkeep_ready = false;
static pthread_mutex_t active_list_mutex;
static int current_log_id = 0;

void * upkeep_run () {
    pthread_mutex_init(&active_list_mutex, NULL);
    pthread_mutex_lock(&active_list_mutex);
    active_logs = create_linked_list();
    dead_logs = create_linked_list();
    pthread_mutex_unlock(&active_list_mutex);
    upkeep_ready = true;

    while (true) {

    }
    return NULL;
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
    success = remove_item(active_logs, expired);

    pthread_mutex_unlock(&active_list_mutex);
    if (success) {
        pthread_mutex_lock(&active_list_mutex);
        add_item(dead_logs, expired);
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