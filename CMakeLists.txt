cmake_minimum_required(VERSION 3.0)

project(zaliczeniejezykowprogramowania LANGUAGES C)
set(CMAKE_C_STANDARD 11)

include_directories(src)
include_directories(src/duthomhas)


add_library(csprng STATIC src/csprng.c)

add_executable(zaliczeniejezykowprogramowania src/main.c)
target_link_libraries(zaliczeniejezykowprogramowania csprng)


install(TARGETS zaliczeniejezykowprogramowania RUNTIME DESTINATION bin)
