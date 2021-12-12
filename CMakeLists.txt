cmake_minimum_required(VERSION 3.20)
project(ex1matam C)

set(CMAKE_C_STANDARD 99)
add_executable(ex1matam matamikya.c matamikya_main.c matamikya_tests.c matamikya_print.c)
set_target_properties(ex1matam PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(ex1matam libas.a)
target_link_libraries(ex1matam libmtm.a)

