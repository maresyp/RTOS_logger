#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

/* PUBLIC DEFINES */

#define LOGGER_LEVEL_INFO 1
#define LOGGER_LEVEL_WARNING 2
#define LOGGER_LEVEL_ERROR 4

typedef const char *const(*dump_function) (void);

typedef struct logger_init_type {
    int logging_level_signal;
    int logging_activation_signal;
    int logging_dump_signal;
    dump_function logging_dump_function;

}logger_init_type;

/* PUBLIC FUNCTIONS DECLARATIONS */

extern int logger_log(const int level, const char * string, ...);
extern void logger_init(const logger_init_type *const logger_init);
extern void logger_destroy();

#endif //LOGGER_LOGGER_H
