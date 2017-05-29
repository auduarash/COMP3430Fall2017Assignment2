#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include "console.h"
#include "player.h"
#include "shared.h"

#define PLAYER_ANIM_TILES 2
#define PLAYER_HEIGHT 2
#define FINAL_JUMP_DIFF 3
#define VERTICAL_JUMP_DIFF 4
static char* PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};
extern pthread_mutex_t player_position_mutex;
extern pthread_mutex_t player_tile_mutex;
extern pthread_mutex_t draw_mutex;
extern bool console_ready;

int player_position_x = 21;
int player_position_y = 38;
int player_max_bound = 21;
int player_min_bound = 2;
static int prev_x = 21;
static int prev_y = 38;
static int player_current_tile = 0;


void set_player_position(int x, int y) {
    if (x != 0) {
        if (player_position_x <= player_min_bound){
            
        } else if (x > 0 && player_position_x == player_max_bound){
            
        } else if (x < 0 && player_position_x == 5) {
            player_position_x += x * FINAL_JUMP_DIFF;
        } else {
            player_position_x += x * VERTICAL_JUMP_DIFF;
        }
    } else if (y != 0) {
        player_position_y += y;
    }
} 

void update_player(int x, int y) {
    pthread_mutex_lock(&player_position_mutex);

    set_player_position(x, y);

    pthread_mutex_lock(&player_tile_mutex);
    char **tile = PLAYER_GRAPHIC[player_current_tile];
    pthread_mutex_unlock(&player_tile_mutex);
    
    pthread_mutex_lock(&draw_mutex);
    consoleClearImage(prev_x, prev_y, PLAYER_HEIGHT, strlen(tile[0]));
    consoleDrawImage(player_position_x, player_position_y, tile, PLAYER_HEIGHT);
    pthread_mutex_unlock(&draw_mutex);
    prev_x = player_position_x;
    prev_y = player_position_y;
    pthread_mutex_unlock(&player_position_mutex);
}


void player_run() {
    //Do not start until console has been initialized
    while ( ! console_ready );

    while (true) {
        pthread_mutex_lock(&player_tile_mutex);
        player_current_tile = (player_current_tile + 1) % 2;
        pthread_mutex_unlock(&player_tile_mutex);
        update_player(0, 0);
        sleepTicks(50);
    }

}