#ifndef LOG_GENERATOR_H
#define LOG_GENERATOR_H


typedef struct LOG_GENERATOR_PARAMS *LogGeneratorParam;
struct LOG_GENERATOR_PARAMS {
    int index;
};

LogGeneratorParam create_log_generator_params(int index);

void *log_generator_run();


#endif