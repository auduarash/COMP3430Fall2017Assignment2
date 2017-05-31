#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "common.h"

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
	thread_ptr new_thread = malloc(sizeof(struct THREADINFO));
	new_thread->thread_num = thread_number;
	new_thread->thread_name = thread_name;
	new_thread->thread_method = thread_method;
	new_thread->thread_arg = arg;
	create_thread(new_thread);
	return new_thread;
}