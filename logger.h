#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

/* PUBLIC DEFINES */

#define LOGGER_LEVEL_MIN 1
#define LOGGER_LEVEL_MID 2
#define LOGGER_LEVEL_MAX 4

/* PUBLIC FUNCTIONS DECLARATIONS */

extern int logger_log(const int level, const char * string, ...);
extern void logger_init();
extern void logger_destroy();

#endif //LOGGER_LOGGER_H
