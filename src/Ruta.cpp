//
// Created by arman on 12/05/2026.
//

#include "../include/Ruta.h"

Ruta::Ruta(const std::string& nombre, const std::string& tipo, bool act, const std::string& descripcion) : nombre(nombre),
tipo(tipo), activa(act), descripcion(descripcion) {}

Ruta::~Ruta() {}

void Ruta::mostrarInformacion() const {
    std::cout << "=== INFORMACION DE LA RUTA ===" << std::endl;
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Tipo:   " << tipo << std::endl;
    std::cout << "Estado: " << (activa ? "Activa" : "Inactiva") << std::endl;
    std::cout << "Desc:   " << descripcion << std::endl;
    std::cout << "------------------------------" << std::endl;
}
