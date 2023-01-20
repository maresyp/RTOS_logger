//
// Created by Mateusz Pysera on 07/01/2023.
//
#include "logger.h"

int main(int argc, char** argv) {
    logger_init();

    logger_log(1, "something");
    logger_log(1, "something2");
    logger_log(1, "something3");

    logger_destroy();

    return 0;
}
