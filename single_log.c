#include <stdlib.h>

#include "single_log.h"

#define LOG_OFFSET 5

static char *LOG_GRAPHIC[] = {
        "/======================\\",
        "",
        "",
        "\\======================/"
};

void * single_log_run( void * args )  {
    SingleLogArgs log_params = (SingleLogArgs) args;

    char **to_draw = LOG_GRAPHIC;
    
    return NULL;
}