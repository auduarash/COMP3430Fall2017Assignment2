#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "draw_screen.h" 
#include "player.h"
#include "keyboard.h"
#include "upkeep.h"
#include "log_generator.h"



bool is_game_over = false;

pthread_mutex_t draw_mutex;
pthread_mutex_t player_position_mutex;
pthread_mutex_t player_tile_mutex;

pthread_cond_t wait_for_console;
pthread_cond_t game_over;





int main(int argc, char**argv) {
	// exampleRun();
	// create_player_thread();
	pthread_mutex_init(&player_position_mutex, NULL);
	pthread_mutex_init(&player_tile_mutex, NULL);
	pthread_mutex_init(&draw_mutex, NULL);
	pthread_cond_init(&game_over, NULL);

	thread_ptr all_threads[8];
	all_threads[0] = create_thread_object(1, "screen refresh thread", draw_screen_run, NULL);
	all_threads[1]  =  create_thread_object(2, "player thread", player_run, NULL);
	all_threads[2]  = create_thread_object(3, "keyboard thread", keyboard_run, NULL);
	all_threads[3]  = create_thread_object(4, "upkeep thread", upkeep_run, NULL);

	/* We need an array of indices since we will be passing the pointers as arguments*/
	int log_index;
	for (log_index = 0; log_index < 4; log_index++) {
		LogGeneratorParam params = create_log_generator_params(log_index);
		all_threads[4+log_index] = create_thread_object(4+log_index, "log generator thread", log_generator_run, params);
	}

	pthread_cond_wait(&game_over, &draw_mutex);
	is_game_over = true;
	int thread_number = 0;
	for (thread_number = 0; thread_number < 8; thread_number++) {
		pthread_join(all_threads[7-thread_number]->thread_id, NULL);
	}
	pthread_mutex_destroy(&player_position_mutex);
	pthread_mutex_destroy(&player_tile_mutex);
	pthread_mutex_destroy(&draw_mutex);
	return 0;
}
