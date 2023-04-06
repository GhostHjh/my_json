cmake_minimum_required(VERSION 3.0.0)

project(my_json_plus)

include_directories(include)

aux_source_directory(src include_src)

add_executable(main main.cpp ${include_src})

add_compile_options(std=c++11)


