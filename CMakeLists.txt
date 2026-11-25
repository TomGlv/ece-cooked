cmake_minimum_required(VERSION 3.26)
project(Projet_Ece_cooked C)

set(CMAKE_C_STANDARD 23)

add_executable(Projet_Ece_cooked main.c  header.c  header.h)

target_link_libraries(Projet_Ece_cooked -lmingw32 -lalleg44)