//
// Created by arman on 21/05/2026.
//

#include "../include/Bus.h"
#include "../include/Conductor.h"

Bus::Bus(std::string placa, const int _cap, Ruta* _rutaA) : placa(std::move(placa)), capacidad(_cap), rutaAsignada(_rutaA){}

void Bus::asignarRuta(Ruta *nuevaRuta) {
    this->rutaAsignada = nuevaRuta;
}

void Bus::desvincularRuta() {
    this->rutaAsignada = nullptr;
}

//Toca aqui definir asignar conductor y desvincular

std::string Bus::placa1() const {
    return this->placa;
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

