//
// Created by arman on 12/05/2026.
//

#include <iostream>
#include "../include/Ruta.h"

Ruta::Ruta(std::string nombre, std::string tipo, const bool act, std::string descripcion) : nombre(std::move(nombre)), tipo(std::move(tipo)), activa(act), descripcion(std::move(descripcion)) {}
//No se usa referencia al pasar los strings, en cambio se usa std::move para evitar copias innecesarias y solo mover las variables

Ruta::~Ruta() = default;

void Ruta::mostrarInformacion() const {
    std::cout << "=== INFORMACION DE LA RUTA ===" << std::endl;
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Tipo:   " << tipo << std::endl;
    std::cout << "Estado: " << (activa ? "Activa" : "Inactiva") << std::endl;
    std::cout << "Desc:   " << descripcion << std::endl;
    std::cout << "------------------------------" << std::endl;
}
