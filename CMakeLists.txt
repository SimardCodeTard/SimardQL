cmake_minimum_required(VERSION 3.10)

project(SimardQL VERSION 0.1)

add_executable(SimardQL src/main.c)

target_compile_options(SimardQL PRIVATE -Wall -Wextra -Wpedantic)
