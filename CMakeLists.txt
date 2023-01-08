cmake_minimum_required(VERSION 3.24)
project(logger C)

set(CMAKE_C_STANDARD 99)

add_library(logger logger.c main.c)
