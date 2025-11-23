cmake_minimum_required(VERSION 3.24)
project(2526_TI301_PJT C)

set(CMAKE_C_STANDARD 11)

add_executable(TI_301_PJT
        main.c cellule.c cellule.h list.c list.h graphe.c graphe.h
        tarjan.c
        tarjan.h
        matrix.c
        matrix.h)

