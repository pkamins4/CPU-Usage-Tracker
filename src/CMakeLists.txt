add_library(Threads STATIC)

target_sources(Threads PUBLIC reader.c)
target_sources(Threads PUBLIC analyzer.c)
target_sources(Threads PUBLIC printer.c)
target_sources(Threads PUBLIC queue.c)

target_include_directories(Threads PUBLIC "${CMAKE_SOURCE_DIR}/lib")

