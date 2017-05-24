#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "console.h"
#include "player.h"

#define PLAYER_ANIM_TILES 2
#define PLAYER_HEIGHT 2
static char* PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};


void player_run() {
    int i;
    while (true) {
        for (i = 0; i < PLAYER_ANIM_TILES; i++) {
            char **tile = PLAYER_GRAPHIC[i];
            consoleDrawImage(10, 10, tile, PLAYER_HEIGHT);
            sleep(1);
            consoleClearImage(10, 10, PLAYER_HEIGHT, strlen(tile[0]));
        }
    }

}