#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#define validate_call(s, error_message) \
	if (s != 0) perror(error_message);


typedef struct THREADINFO *thread_ptr;
struct THREADINFO {
	pthread_t thread_id;
	void *thread_method;
	void *thread_arg;
};

void create_thread(thread_ptr info);

thread_ptr create_thread_object(void *thread_method, void *arg);


#endif