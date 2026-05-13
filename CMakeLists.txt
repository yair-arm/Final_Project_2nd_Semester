cmake_minimum_required(VERSION 3.22)
project(Proyecto)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(Proyecto main.cpp
        src/Ruta.cpp
        include/Ruta.h
)
