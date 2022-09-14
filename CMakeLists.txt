cmake_minimum_required(VERSION 3.24.1)

project(CUT)

add_executable(${PROJECT_NAME} main.c queue.c reader.c analyzer.c printer.c logger.c watchdog.c)
