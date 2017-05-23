
#include <string.h>
#include <unistd.h>

#include "console.h"
#include "example.h"

#define GAME_ROWS 24
#define GAME_COLS 80

/**** DIMENSIONS MUST MATCH the ROWS/COLS */

char *GAME_BOARD[] = {
"                                   Lives:",
"/------\\          /------\\          /------\\          /------\\          /------\\",
"|      |          |      |          |      |          |      |          |      |",
"+      +----------+      +----------+      +----------+      +----------+      +",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"",
"",
"",
"" };

#define PLAYER_ANIM_TILES 2
#define PLAYER_HEIGHT 2
static char* PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};

void exampleRun()
{
	int i;
	int offset = 1;
	if (consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD))
	{
		while (true){
			for (i = 0; i<PLAYER_ANIM_TILES; i++)
			{
				char** tile = PLAYER_GRAPHIC[i];
				consoleDrawImage(10 + offset, 10 + offset, tile, PLAYER_HEIGHT);
				consoleRefresh();
				sleep(1);
				consoleClearImage(10 + offset,10 + offset, PLAYER_HEIGHT, strlen(tile[0]));
			}
			offset *= -1;
		}

		finalKeypress(); /* wait for final key before killing curses and game */
	}       
	consoleFinish();        	
}

