//
// Created by arman on 12/05/2026.
//

#include "RutaBarrio.h"

RutaBarrio::RutaBarrio(std::string nombre, const bool act, std::string descripcion, std::string  sec)
: Ruta(std::move(nombre), "De barrio", act, std::move(descripcion)), sector(std::move(sec)){}

RutaBarrio::~RutaBarrio() {
    for (const auto* horario : horarios) {
        delete horario;
    }
}

void RutaBarrio::agregarHorario(Horario* nuevoHorario) {
    this->horarios.push_back(nuevoHorario);
}
const std::vector<Horario*>& RutaBarrio::getHorarios() const {
    return this->horarios;
}

std::string RutaBarrio::getSector() const {
    return sector;
}
