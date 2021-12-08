cmake_minimum_required(VERSION 3.20)
project(ex1matam C)

set(CMAKE_C_STANDARD 99)

add_executable(ex1matam amount_set_str_main.c amount_set_str_tests.c amount_set_str.c matamikya.c)
