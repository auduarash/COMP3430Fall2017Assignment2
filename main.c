#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include "draw_screen.h" 
#include "player.h"
#include "keyboard.h"
#include "upkeep.h"
#include "log_generator.h"


#define validate_call(s, error_message) \
	if (s != 0) perror(error_message);

bool console_ready = false;

pthread_mutex_t draw_mutex;
pthread_mutex_t player_position_mutex;
pthread_mutex_t player_tile_mutex;

pthread_cond_t wait_for_console;

typedef struct thread_info *thread_ptr;
struct thread_info {
	pthread_t thread_id;
	int thread_num;
	char *thread_name;
	void *thread_method;
	void *res;
	void *thread_arg;
};

/*
	Creates a thread given a struct pointing to the thread
*/
void create_thread(thread_ptr info) {
	pthread_attr_t attr;
	int s = pthread_attr_init(&attr);
	validate_call(s, "pthread_init");
	s = pthread_create(&info->thread_id, &attr, info->thread_method, info->thread_arg);
	validate_call(s, "pthread_create");
}


thread_ptr create_thread_object(int thread_number, char *thread_name, void *thread_method, void *arg) {
	thread_ptr new_thread = malloc(sizeof(struct thread_info));
	new_thread->thread_num = thread_number;
	new_thread->thread_name = thread_name;
	new_thread->thread_method = thread_method;
	new_thread->thread_arg = arg;
	create_thread(new_thread);
	return new_thread;
}



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

	thread_ptr log_generator_threads[4];
	/* We need an array of indices since we will be passing the pointers as arguments*/
	int log_generator_indices[4];
	int log_index;
	for (log_index = 0; log_index < 4; log_index++) {
		log_generator_indices[log_index] = log_index;
		log_generator_threads[log_index] = 
			create_thread_object(4+log_index, "log generator thread", log_generator_run, log_generator_indices+log_index);
	}

	s = pthread_join(screen_thread->thread_id, &(screen_thread->res));
	validate_call(s, "pthread_join");
	s = pthread_join(player_thread->thread_id, &(player_thread->res));
	validate_call(s, "pthread_join");
	s = pthread_join(keyboard_thread->thread_id, &(keyboard_thread->res));
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
