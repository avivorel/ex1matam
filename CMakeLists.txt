cmake_minimum_required(VERSION 3.20)
project(ex1matam C)

set(CMAKE_C_STANDARD 99)

add_executable(ex1matam Orders.h Orders.c amount_set.h set.h)
