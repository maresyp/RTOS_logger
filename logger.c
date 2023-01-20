#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>

/* PRIVATE DEFINES */

/* STATIC VARIABLES */

static pthread_mutex_t mutex;
static volatile sig_atomic_t sig_logging_level = LOGGER_LEVEL_INFO;
static FILE * logger_file = NULL;

/* PRIVATE FUNCTIONS DECLARATIONS */

static void siginfo_handler(int signo, siginfo_t *info, void *other);

/* PRIVATE FUNCTIONS DEFINITIONS */

static void siginfo_handler(int signo, siginfo_t *info, void *other) {
    sig_logging_level = info->si_value.sival_int;
}

/* PUBLIC FUNCTIONS DEFINITIONS */
extern int logger_log(const int level, const char * string, ...) {
    va_list args;
    int count = -1;

    va_start(args, string);
    if (level <= sig_logging_level) {
        pthread_mutex_lock(&mutex);
        count = vfprintf(logger_file, string, args);
        pthread_mutex_unlock(&mutex);
    }
    va_end(args);

    return count;
}

extern void logger_init() {
    if (logger_file == NULL) {

        if (pthread_mutex_init(&mutex, NULL) != 0 ) {
            printf("Failed to create mutex");
            exit(EXIT_FAILURE);
        }

        logger_file = fopen("filename.log", "w");
        if (logger_file == NULL) {
            printf("Failed to open file");
            exit(EXIT_FAILURE);
        }

        struct sigaction sa = {
                .sa_sigaction = siginfo_handler,
                .sa_flags = SA_SIGINFO
        };
        sigfillset(&sa.sa_mask);
        sigaction(SIGINFO, &sa, NULL);

    } else {
        printf("logger already initialized");
        exit(EXIT_FAILURE);
    }
}

extern void logger_destroy() {
    pthread_mutex_destroy(&mutex);
}
