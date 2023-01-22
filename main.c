//
// Created by Mateusz Pysera on 07/01/2023.
//
#include "logger.h"
#include <signal.h>

int main(int argc, char** argv) {

    logger_init_type log_init = {
            .logging_dump_signal = SIGUSR1,
            .logging_activation_signal = SIGUSR2,
            .logging_level_signal = SIGINFO,
    };

    logger_init(&log_init);

    logger_log(1, "something");
    logger_log(1, "something2");
    logger_log(1, "something3");

    logger_destroy();

    return 0;
}
