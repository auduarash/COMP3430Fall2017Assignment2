#ifndef SINGLE_LOG_C
#define SINGLE_LOG_C



typedef struct SINGLE_LOG_ARGS *SingleLogArgs;
struct SINGLE_LOG_ARGS {
    int row;
    int direction;
    int refresh_ticks;
};

/*
    Method: single_log_run
    Arguments: log_id

    Logs are reusable. The log_id is representative of the location
        of the log on the screen. Direction is either positive or
        negative representing if the log moves to the right (+ve)
        of to the left (-ve).
*/



void *single_log_run ( void * args);


#endif