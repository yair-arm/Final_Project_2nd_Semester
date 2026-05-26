//
// Created by arman on 21/05/2026.
//

#include "../include/Bus.h"
#include "../include/Conductor.h"

Bus::Bus(std::string placa, const int _cap, Ruta* _rutaA, bool _disponible, std::string _obser) :
placa(std::move(placa)), capacidad(_cap), rutaAsignada(_rutaA), disponible(_disponible), observacion(std::move(_obser)) {}

void Bus::asignarRuta(Ruta *nuevaRuta) {
    this->rutaAsignada = nuevaRuta;
}

void Bus::desvincularRuta() {
    this->rutaAsignada = nullptr;
}

void Bus::asignarConductor(Conductor *nuevoConductor) {
    this->conductorAsignado = nuevoConductor;
}

void Bus::desvincularConductor() {
    this->conductorAsignado = nullptr;
}

std::string Bus::placa1() const {
    return this->placa;
}

bool Bus::disponible1() const {
    return disponible;
}

std::string Bus::observacion1() const {
    return observacion;
}

int Bus::capacidad1() const {
    return this->capacidad;
}

Ruta * Bus::ruta_asignada() const {
    return this->rutaAsignada;
}

Conductor * Bus::conductor_asignado() const {
    return conductorAsignado;
}

