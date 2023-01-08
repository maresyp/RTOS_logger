//
// Created by Mateusz Pysera on 07/01/2023.
//
#include "logger.h"

int main(int argc, char** argv) {
    logger_init();

    logger_log();

    logger_destroy();
    return 0;
}
