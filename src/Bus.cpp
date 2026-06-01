//
// Created by arman on 21/05/2026.
//

#include "../include/Bus.h"
#include "../include/Conductor.h"

Bus::Bus(std::string placa, const int _cap, Ruta* _rutaA, const bool _disponible, std::string _obser) :
placa(std::move(placa)), capacidad(_cap), rutaAsignada(_rutaA), disponible(_disponible), observacion(std::move(_obser)) {}

Bus::~Bus() {
    delete conductorAsignado;
}

void Bus::asignarRuta(Ruta *nuevaRuta) {
    this->rutaAsignada = nuevaRuta;
}

void Bus::desvincularRuta() {
    this->rutaAsignada = nullptr;
}

void Bus::asignarConductor(Conductor* nuevoConductor) {
    if (conductorAsignado != nullptr) {
        delete conductorAsignado;
    }
    conductorAsignado = nuevoConductor;

    if (nuevoConductor != nullptr) {
        nuevoConductor->asignarBus(this);
    }
}

void Bus::desvincularConductor() {
    delete this->conductorAsignado;
    this->conductorAsignado = nullptr;
}

std::string Bus::getPlaca() const {
    return this->placa;
}

bool Bus::isDisponible() const {
    return disponible;
}

std::string Bus::getObservacion() const {
    return observacion;
}

int Bus::getCapacidad() const {
    return this->capacidad;
}

Ruta * Bus::getRutaAsignada() const {
    return this->rutaAsignada;
}

Conductor * Bus::getConductorAsignado() const {
    return conductorAsignado;
}

