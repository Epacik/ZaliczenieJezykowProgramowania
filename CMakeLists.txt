cmake_minimum_required(VERSION 3.0)

project(zaliczeniejezykowprogramowania LANGUAGES C)

include_directories(src)
include_directories(src/duthomhas)

add_library(subreg src/subreg.c)
add_library(csprng src/csprng.c)
add_executable(zaliczeniejezykowprogramowania src/main.c)


install(TARGETS zaliczeniejezykowprogramowania RUNTIME DESTINATION bin)
