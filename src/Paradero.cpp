//
// Created by arman on 21/05/2026.
//

#include "../include/Paradero.h"
#include <iostream>

Paradero::Paradero(std::string nom, std::string ubi) : nombre(std::move(nom)), ubicacion(std::move(ubi)) {}
void Paradero::mostrarDetalleParadero() const{
    std::cout << nombre << ", " << ubicacion << std::endl;
}

std::string Paradero::nombre1() const {
    return nombre;
}

std::string Paradero::ubicacion1() const {
    return ubicacion;
}
