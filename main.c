#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include "draw_screen.h" 
#include "player.h"
#include "keyboard.h"


#define validate_call(s, error_message) \
	if (s != 0) perror(error_message);


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


void create_draw_screen_thread(thread_ptr screen_thread) {
	screen_thread->thread_method = draw_screen_run;
	create_thread(screen_thread);
}

void create_player_thread(thread_ptr screen_thread) {
	screen_thread->thread_method = player_run;
	create_thread(screen_thread);
}

void create_keyboard_thread(thread_ptr keyboard_thread) {
	keyboard_thread->thread_method = keyboard_run;
	create_thread(keyboard_thread);
}

thread_ptr create_thread_object(int thread_number, char *thread_name) {
	thread_ptr new_thread = malloc(sizeof(struct thread_info));
	new_thread->thread_num = thread_number;
	new_thread->thread_name = thread_name;
	
	return new_thread;
}



int main(int argc, char**argv) {
	// exampleRun();
	// create_player_thread();
	thread_ptr screen_thread = create_thread_object(1, "screen refresh thread");
	create_draw_screen_thread(screen_thread);
	thread_ptr player_thread = create_thread_object(2, "player thread");
	create_player_thread(player_thread);
	thread_ptr keyboard_thread = create_thread_object(3, "keyboard thread");
	create_keyboard_thread(keyboard_thread);
	int s = pthread_join(screen_thread->thread_id, &screen_thread->res);
	validate_call(s, "pthread_join");
	s = pthread_join(player_thread->thread_id, &player_thread->res);
	validate_call(s, "pthread_join");
	s = pthread_join(keyboard_thread->thread_id, &keyboard_thread->res);
	validate_call(s, "pthread_join");
	return 0;
}
