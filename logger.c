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

static pthread_mutex_t file_write_mutex;
static sem_t dump_semaphore;
static volatile sig_atomic_t sig_logging_level = LOGGER_LEVEL_INFO;
static volatile sig_atomic_t sig_logging_active = 1;
static FILE * logger_file = NULL;
static volatile sig_atomic_t dump_counter = 0;

/* PRIVATE FUNCTIONS DECLARATIONS */

static void sig_log_level_change_handler(int signo, siginfo_t *info, void *other);
static void sig_log_dump_handler(int signo, siginfo_t *info, void *other);
static void sig_log_activation_handler(int signo, siginfo_t *info, void *other);

/* PRIVATE FUNCTIONS DEFINITIONS */

static void sig_log_level_change_handler(int signo, siginfo_t *info, void *other) {
    sig_logging_level = info->si_value.sival_int;
}

static void sig_log_activation_handler(int signo, siginfo_t *info, void *other) {
    sig_logging_active = info->si_value.sival_int;
}

static void sig_log_dump_handler(int signo, siginfo_t *info, void *other) {
    sem_post(&dump_semaphore);
}

static void* dump_thread(void* arg) {
    while (true) {
        while (sem_wait(&dump_semaphore) && (errno == EINTR));
        dump_counter++;

        FILE * dump_file;
        dump_file = fopen("dump_file", "w");
        if (dump_file == NULL) {
            return NULL;
        }

        fprintf(dump_file, ((logger_init_type*)arg)->logging_dump_function());
    }
}

/* PUBLIC FUNCTIONS DEFINITIONS */
extern int logger_log(const int level, const char * string, ...) {
    va_list args;
    int count = -1;

    va_start(args, string);
    if (sig_logging_active && level <= sig_logging_level ) {
        pthread_mutex_lock(&file_write_mutex);
        count = vfprintf(logger_file, string, args);
        pthread_mutex_unlock(&file_write_mutex);
    }
    va_end(args);

    return count;
}

extern void logger_init(const logger_init_type *const logger_init) {
    if (logger_file == NULL) {
        pthread_t tid;

        if (pthread_mutex_init(&file_write_mutex, NULL) != 0 ) {
            printf("Failed to create mutex");
            exit(EXIT_FAILURE);
        }

        sem_init(&dump_semaphore, 0, 0);

        pthread_create(&tid, NULL, dump_thread, (void*)logger_init);

        logger_file = fopen("filename.log", "w");
        if (logger_file == NULL) {
            printf("Failed to open file");
            exit(EXIT_FAILURE);
        }

        // Handle level change
        struct sigaction sa = {
                .sa_sigaction = sig_log_level_change_handler,
                .sa_flags = logger_init->logging_level_signal
        };
        sigfillset(&sa.sa_mask);
        sigaction(logger_init->logging_level_signal, &sa, NULL);

        // Handle on / off of logger
        sa.sa_sigaction = sig_log_activation_handler;
        sa.sa_flags = logger_init->logging_activation_signal;
        sigfillset(&sa.sa_mask);
        sigaction(logger_init->logging_activation_signal, &sa, NULL);

        // Handle dump
        sa.sa_sigaction = sig_log_dump_handler;
        sa.sa_flags = logger_init->logging_dump_signal;
        sigfillset(&sa.sa_mask);
        sigaction(logger_init->logging_dump_signal, &sa, NULL);

    } else {
        printf("logger already initialized");
        exit(EXIT_FAILURE);
    }
}

extern void logger_destroy() {
    pthread_mutex_destroy(&file_write_mutex);
    sem_destroy(&dump_semaphore);
}
