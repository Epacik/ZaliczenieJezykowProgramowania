cmake_minimum_required(VERSION 3.0)

project(zaliczeniejezykowprogramowania LANGUAGES C)

add_executable(zaliczeniejezykowprogramowania main.c)

install(TARGETS zaliczeniejezykowprogramowania RUNTIME DESTINATION bin)
