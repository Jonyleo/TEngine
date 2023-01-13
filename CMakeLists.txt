cmake_minimum_required(VERSION 3.0.0)
project(TEngine VERSION 0.1.0)

add_definitions(-DDEBUG)
#add_definitions(-DPERF_CHECK)

add_subdirectory(core)
add_subdirectory(tangram)