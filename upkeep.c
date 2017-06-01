#include <stdlib.h>
#include "console.h"
#include "list.h"
#include "upkeep.h"

/*
    Upkeep helps handle memory for objects that come and go as the program progresses
*/
static LinkedList active_logs;
static LinkedList dead_logs;
static bool upkeep_ready = false;

void * upkeep_run () {
    active_logs = create_linked_list();
    dead_logs = create_linked_list();
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
        new = remove_front(dead_logs);
    }
    add_item(active_logs, new);
    return new;
}