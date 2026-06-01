//
// Created by Nicolle on 5/26/2026.
//

#include "../include/IncidenciaExcepcion.h"

IncidenciaExcepcion::IncidenciaExcepcion(std::string condicion, std::string mensaje)
    : condicion(std::move(condicion)), mensaje(std::move(mensaje)) {}

IncidenciaExcepcion::~IncidenciaExcepcion() = default;

std::string IncidenciaExcepcion::infoExcepcion() const {
    return "[" + condicion + "] " + mensaje;
}

const char* IncidenciaExcepcion::what() const noexcept {
    return mensaje.c_str();
}