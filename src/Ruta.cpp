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
    os << "Nombre: " << ruta.getName() << std::endl;
    os << "Tipo: " << ruta.getType() << std::endl;
    os << "Activa: " << (ruta.isActive() ? "Sí" : "No") << std::endl;
    os << "Descripcion: " << ruta.getDescription() << std::endl;
    return os;
}


const std::vector<Paradero *>& Ruta::getParaderos() const {
    return paraderos;
}
std::string Ruta::getName() const {
    return nombre;
}
std::string Ruta::getType() const {
    return tipo;
}
bool Ruta::isActive() const {
    return activa;
}
std::string Ruta::getDescription() const {
    return descripcion;
}

bool Ruta::operator<(const Ruta &other) const {
    if (nombre != other.nombre) return nombre < other.nombre;
    return tipo < other.tipo;
}

bool Ruta::operator==(const Ruta &other) const {
    return nombre == other.nombre && tipo == other.tipo;
}

