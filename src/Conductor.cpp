//
// Created by arman on 21/05/2026.
//

#include "../include/Conductor.h"

Conductor::Conductor(std::string idConductor, std::string nombre, std::string licencia, std::string telefono) :
idConductor (std::move(idConductor)), nombre (std::move(nombre)),licencia (std::move(licencia)), telefono (std::move(telefono)){}

std::string Conductor::getId() const {
    return idConductor;
}

std::string Conductor::getName() const {
    return nombre;
}

std::string Conductor::getLicencia() const {
    return licencia;
}

std::string Conductor::getTelefono() const {
    return telefono;
}

Bus* Conductor::getBusAsignado() const {
    return busAsignado;
}

void Conductor::asignarBus(Bus* nuevoBus) {
    busAsignado = nuevoBus;
}

void Conductor::desvincularBus() {
    this->busAsignado = nullptr;
}

