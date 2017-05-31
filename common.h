#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#define validate_call(s, error_message) \
	if (s != 0) perror(error_message);


typedef struct THREADINFO *thread_ptr;
struct THREADINFO {
	pthread_t thread_id;
	int thread_num;
	char *thread_name;
	void *thread_method;
	void *res;
	void *thread_arg;
};

void create_thread(thread_ptr info);

thread_ptr create_thread_object(int thread_number, char *thread_name, void *thread_method, void *arg);


#endif