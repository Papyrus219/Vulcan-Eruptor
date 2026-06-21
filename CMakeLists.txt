cmake_minimum_required(VERSION 3.10)

project(eruptor)

add_executable(eruptor main.cpp)

install(TARGETS eruptor RUNTIME DESTINATION bin)
