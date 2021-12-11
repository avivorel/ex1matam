cmake_minimum_required(VERSION 3.20)
project(ex1matam C)

set(CMAKE_C_STANDARD 99)
add_executable(ex1matam amount_set.h set.h matamikya.h matamikya.c)
link_directories(..)
target_link_libraries(ex1matam mtm as)
