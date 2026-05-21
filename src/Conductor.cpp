//
// Created by arman on 21/05/2026.
//

#include "../include/Conductor.h"

Conductor::Conductor(std::string idConductor, std::string nombre, std::string licencia, std::string telefono) :
idConductor (std::move(idConductor)), nombre (std::move(nombre)),licencia (std::move(licencia)), telefono (std::move(telefono)){}

std::string Conductor::id_conductor() const {
    return idConductor;
}

std::string Conductor::nombre1() const {
    return nombre;
}

std::string Conductor::licencia1() const {
    return licencia;
}

std::string Conductor::telefono1() const {
    return telefono;
}

Bus* Conductor::busAsignado1() const {
    return busAsignado;
}

void Conductor::asignarBus(Bus* nuevoBus) {
    busAsignado = nuevoBus;
}

void Conductor::desvincularBus() {
    this->busAsignado = nullptr;
}

