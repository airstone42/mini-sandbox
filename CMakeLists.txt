cmake_minimum_required(VERSION 3.15)
project(mini_sandbox C)

set(CMAKE_C_STANDARD 11)

add_executable(mini-sandbox src/main.c src/jail.c src/jail.h src/process.c src/process.h src/cli.c src/cli.h)

target_link_libraries(mini-sandbox dl seccomp)
