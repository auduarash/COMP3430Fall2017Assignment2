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



bool console_ready = false;

pthread_mutex_t draw_mutex;
pthread_mutex_t player_position_mutex;
pthread_mutex_t player_tile_mutex;

pthread_cond_t wait_for_console;





int main(int argc, char**argv) {
	// exampleRun();
	// create_player_thread();
	int s;
	pthread_mutex_init(&player_position_mutex, NULL);
	pthread_mutex_init(&player_tile_mutex, NULL);
	pthread_mutex_init(&draw_mutex, NULL);
	pthread_cond_init(&wait_for_console, NULL);

	thread_ptr screen_thread = create_thread_object(1, "screen refresh thread", draw_screen_run, NULL);
	thread_ptr player_thread = create_thread_object(2, "player thread", player_run, NULL);
	thread_ptr keyboard_thread = create_thread_object(3, "keyboard thread", keyboard_run, NULL);
	thread_ptr upkeep_thread = create_thread_object(4, "upkeep thread", upkeep_run, NULL);

	thread_ptr log_generator_threads[4];
	/* We need an array of indices since we will be passing the pointers as arguments*/
	int log_index;
	for (log_index = 0; log_index < 4; log_index++) {
		LogGeneratorParam params = create_log_generator_params(log_index);
		log_generator_threads[log_index] = 
			create_thread_object(4+log_index, "log generator thread", log_generator_run, params);
	}

	s = pthread_join(screen_thread->thread_id, &(screen_thread->res));
	validate_call(s, "pthread_join");
	s = pthread_join(player_thread->thread_id, &(player_thread->res));
	validate_call(s, "pthread_join");
	s = pthread_join(keyboard_thread->thread_id, &(keyboard_thread->res));
	validate_call(s, "pthread_join");
	s = pthread_join(upkeep_thread->thread_id, &(upkeep_thread->res));
	validate_call(s, "pthread_join");
	for (log_index = 0; log_index < 4; log_index++) {
		thread_ptr curr = log_generator_threads[log_index];
		s = pthread_join(curr->thread_id, &(curr->res));
		validate_call(s, "pthread_join");
	}

	pthread_mutex_destroy(&player_position_mutex);
	pthread_mutex_destroy(&player_tile_mutex);
	pthread_mutex_destroy(&draw_mutex);
	pthread_exit(NULL);
	return 0;
}
