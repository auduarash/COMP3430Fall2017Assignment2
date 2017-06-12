#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include "draw_screen.h"
#include "console.h"
#include "player.h"
#include "shared.h"
#include "upkeep.h"

#define PLAYER_ANIM_TILES 2
#define PLAYER_HEIGHT 2
#define OUT_OF_BOUNDS 21
#define STAGE_ONE 17
#define STAGE_TWO 13
#define STAGE_THREE 9
#define STAGE_FOUR 5
#define END_OF_GAME 2
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

static int player_row = 21;
static int player_column = 38;
int player_max_bound = 21;
int player_min_bound = 2;
static int prev_row = 21;
static int prev_column = 38;
static int player_current_tile = 0;


void set_player_position(int x, int y) {
    if (x != 0) {
        if (x > 0) {
            switch (player_row) {
                case STAGE_FOUR: player_row = STAGE_THREE; break;
                case STAGE_THREE: player_row = STAGE_TWO; break;
                case STAGE_TWO: player_row = STAGE_ONE; break;
                case STAGE_ONE: player_row = OUT_OF_BOUNDS; break;
            }
        } else {
            switch (player_row) {
                case OUT_OF_BOUNDS: player_row = STAGE_ONE; break;
                case STAGE_ONE: player_row = STAGE_TWO; break;
                case STAGE_TWO: player_row = STAGE_THREE; break;
                case STAGE_THREE: player_row = STAGE_FOUR; break;
                case STAGE_FOUR: {
                    int player_y_base = player_column % 18;
                    if (1 <= player_y_base && player_y_base <= 5) {
                        player_row = END_OF_GAME; 
                    }
                    break;
                }
            }
        }
    } else if (y != 0) {
        int new_y = player_column + y;
        if (new_y >= 0 && new_y < GAME_COLS-2){
            player_column = new_y;
        }
    }
} 


void update_player(int x, int y) {
    pthread_mutex_lock(&player_position_mutex);

    set_player_position(x, y);

    pthread_mutex_lock(&player_tile_mutex);
    char **tile = PLAYER_GRAPHIC[player_current_tile];
    pthread_mutex_unlock(&player_tile_mutex);
    
    pthread_mutex_lock(&draw_mutex);
    consoleClearImage(prev_row, prev_column, PLAYER_HEIGHT, strlen(tile[0]));
    consoleDrawImage(player_row, player_column, tile, PLAYER_HEIGHT);
    prev_row = player_row;
    prev_column = player_column;
    pthread_mutex_unlock(&draw_mutex);
    verify_player_position();
    pthread_mutex_unlock(&player_position_mutex);
}

void verify_player_position() {
    switch (player_row) {
        case OUT_OF_BOUNDS: break;
        case END_OF_GAME: {
            place_player_on_log(5, 0);
            frog_crossed_pond();
            reset_player_position();
            break; //TODO: Make this great again
        }
        default: {
            int row = (player_row - 5) / 4;
            bool player_found_log = place_player_on_log(row, player_column);

            if ( ! player_found_log ) {
                live_lost();
                reset_player_position();
            }
        }
    }
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