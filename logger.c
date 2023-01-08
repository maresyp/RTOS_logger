#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <stdarg.h>

/* DEFINES */

#define LOGGER_LEVEL_MIN 1
#define LOGGER_LEVEL_MIN 2
#define LOGGER_LEVEL_MIN 4

/* STATIC VARIABLES */
static sem_t siginfo_sem;
static FILE * logger_file = NULL;

/* PRIVATE FUNCTIONS DECLARATIONS */

static void* siginfo_thread(void* arg);
static void siginfo_handler(int signo, siginfo_t *info, void *other);

/* PRIVATE FUNCTIONS DEFINITIONS */

static void siginfo_handler(int signo, siginfo_t *info, void *other) {
    sem_post(&siginfo_sem);
}

static void* siginfo_thread(void* arg) {
    while (1) {
        while(sem_wait(&siginfo_sem) && (errno == EINTR));
        // handle change of logging level
    }
}

/* PUBLIC FUNCTIONS DEFINITIONS */
extern int logger_log(const int level, const char * string, ...) {
    va_list args;
    int count = -1;

    va_start(args, string);
    if (level < 0) {
        count = vfprintf(logger_file, string, args);
    }
    va_end(args);

    return count;
}

extern void logger_init() {
    if (NULL == logger_file) {
        sem_init(&siginfo_sem, 0, 0);
    }
}

extern void logger_destroy() {
    sem_destroy(&siginfo_sem);
}
