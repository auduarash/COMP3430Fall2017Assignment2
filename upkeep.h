#ifndef UPKEEP_H
#define UPKEEP_H

#include "list.h"

void *upkeep_run();

Log get_new_log();

void delete_log(Log log);

bool place_player_on_log(int row, int player_y_position);

void live_lost();

void frog_crossed_pond();

void join_log_threads();

#endif