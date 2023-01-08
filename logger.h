#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

extern int logger_log(const int level, const char * string, ...);
extern void logger_init();
extern void logger_destroy();

#endif //LOGGER_LOGGER_H
