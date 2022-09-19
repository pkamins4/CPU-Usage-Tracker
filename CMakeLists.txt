cmake_minimum_required(VERSION 3.24.1)

project(CUT)

set(SOURCE
	./src/main.c
	./src/analyzer.c
	./src/logger.c
	./src/printer.c
	./src/queue.c
	./src/reader.c
	./src/watchdog.c
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(CUT
	PRIVATE
		${PROJECT_SOURCE_DIR}/include
)
