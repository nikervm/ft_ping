cmake_minimum_required(VERSION 3.17)
project(ft_ping C)

include_directories(incs)
set(CMAKE_C_STANDARD 99)

add_executable(ft_ping srcs/main.c srcs/utils.c srcs/init.c srcs/signals.c srcs/ping.c)