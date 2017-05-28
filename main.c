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


#define validate_call(s, error_message) \
	if (s != 0) perror(error_message);


pthread_mutex_t draw_mutex;
pthread_mutex_t player_position_mutex;
pthread_mutex_t player_tile_mutex;

typedef struct thread_info *thread_ptr;
struct thread_info {
	pthread_t thread_id;
	int thread_num;
	char *thread_name;
	void *thread_method;
	void *res;
};

/*
	Creates a thread given a struct pointing to the thread
*/
void create_thread(thread_ptr info) {
	pthread_attr_t attr;
	int s = pthread_attr_init(&attr);
	validate_call(s, "pthread_init");
	s = pthread_create(&info->thread_id, &attr, info->thread_method, info);
	validate_call(s, "pthread_create");
}


thread_ptr create_thread_object(int thread_number, char *thread_name, void *thread_method) {
	thread_ptr new_thread = malloc(sizeof(struct thread_info));
	new_thread->thread_num = thread_number;
	new_thread->thread_name = thread_name;
	new_thread->thread_method = thread_method;
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

	thread_ptr screen_thread = create_thread_object(1, "screen refresh thread", draw_screen_run);
	thread_ptr player_thread = create_thread_object(2, "player thread", player_run);
	thread_ptr keyboard_thread = create_thread_object(3, "keyboard thread", keyboard_run);

	s = pthread_join(screen_thread->thread_id, &screen_thread->res);
	validate_call(s, "pthread_join");
	s = pthread_join(player_thread->thread_id, &player_thread->res);
	validate_call(s, "pthread_join");
	s = pthread_join(keyboard_thread->thread_id, &keyboard_thread->res);
	validate_call(s, "pthread_join");

	pthread_mutex_destroy(&player_position_mutex);
	pthread_mutex_destroy(&player_tile_mutex);
	pthread_mutex_destroy(&draw_mutex);
	pthread_exit(NULL);
	return 0;
}
