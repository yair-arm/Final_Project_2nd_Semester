//
// Created by arman on 12/05/2026.
//

#include <iostream>
#include "../include/Ruta.h"

Ruta::Ruta(std::string nombre, std::string tipo, const bool act, std::string descripcion) : nombre(std::move(nombre)),
    tipo(std::move(tipo)), activa(act), descripcion(std::move(descripcion)) {
}

//No se usa referencia al pasar los strings, en cambio, se usa std::move para evitar copias innecesarias y solo mover las variables

Ruta::~Ruta() {
    for (const auto* paradero : paraderos) {
        delete paradero;
    }
    paraderos.clear();
}

void Ruta::agregarParadero(Paradero* nuevoParadero) {
    paraderos.push_back(nuevoParadero);
}

std::ostream& operator<<(std::ostream& os, const Ruta& ruta) {
    os << "Nombre: " << ruta.nombre1() << std::endl;
    os << "Tipo: " << ruta.tipo1() << std::endl;
    os << "Activa: " << (ruta.activa1() ? "Sí" : "No") << std::endl;
    os << "Descripcion: " << ruta.descripcion1() << std::endl;
    return os;
}

const std::vector<Paradero *>& Ruta::paraderos1() const {
    return paraderos;
}
std::string Ruta::nombre1() const {
    return nombre;
}
std::string Ruta::tipo1() const {
    return tipo;
}
bool Ruta::activa1() const {
    return activa;
}
std::string Ruta::descripcion1() const {
    return descripcion;
}
