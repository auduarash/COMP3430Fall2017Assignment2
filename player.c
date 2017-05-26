#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "console.h"
#include "player.h"
#include "shared.h"

#define PLAYER_ANIM_TILES 2
#define PLAYER_HEIGHT 2
static char* PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};


int player_position_x = 10;
int player_position_y = 10;
static int prev_x = 10;
static int prev_y = 10;
static int player_current_tile = 0;

void update_player(int x, int y) {
    player_position_x += x;
    player_position_y += y;
    char **tile = PLAYER_GRAPHIC[player_current_tile];
    consoleClearImage(prev_x, prev_y, PLAYER_HEIGHT, strlen(tile[0]));
    consoleDrawImage(player_position_x, player_position_y, tile, PLAYER_HEIGHT);
    prev_x = player_position_x;
    prev_y = player_position_y;
}


void player_run() {
    while (true) {
        player_current_tile += 1;
        player_current_tile %= 2;
        sleepTicks(25);
        update_player(0, 0);
    }

}