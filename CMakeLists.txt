cmake_minimum_required(VERSION 3.22)
project(Proyecto)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(Proyecto main.cpp
        src/Ruta.cpp
        include/Ruta.h
        src/RutaCentro.cpp
        include/RutaCentro.h
        src/RutaBarrio.cpp
        include/RutaBarrio.h
        include/Horario.h
        src/Horario.cpp
        src/Paradero.cpp
        include/Paradero.h
        src/Bus.cpp
        include/Bus.h
        src/Conductor.cpp
        include/Conductor.h
)
