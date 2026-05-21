//
// Created by arman on 20/05/2026.
//
#include "Horario.h"
#include <iostream>

Horario::Horario(std::string hor, std::string min, std::string est) :
hora(std::move (hor)), minuto(std::move (min)), estado(std::move(est)){}

void Horario::actualizarEstado(std::string nuevoEstado) {
    this->estado = std::move(nuevoEstado);
}

std::string Horario::hora1() const {
    return hora;
}
std::string Horario::minuto1() const {
    return minuto;
}
std::string Horario::estado1() const {
    return estado;
}

Horario::~Horario() = default;
