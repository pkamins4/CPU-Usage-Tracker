cmake_minimum_required(VERSION 3.24.1)

project(CUT)

add_executable(${PROJECT_NAME} src/main.c src/queue.c src/reader.c src/analyzer.c src/printer.c src/logger.c 
	src/watchdog.c)
