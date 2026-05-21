//
// Created by arman on 12/05/2026.
//

#include "RutaBarrio.h"

RutaBarrio::RutaBarrio(const std::string &nombre, const bool act, const std::string &descripcion, std::string  sec)
: Ruta(nombre, "De barrio", act, descripcion), sector(std::move(sec)){}

void RutaBarrio::agregarHorario(Horario* nuevoHorario) {
    this->horarios.push_back(nuevoHorario);
}
const std::vector<Horario*>& RutaBarrio::getHorarios() const {
    return this->horarios;
}

std::string RutaBarrio::sector1() const {
    return sector;
}
