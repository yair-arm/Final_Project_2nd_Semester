//
// Created by arman on 26/05/2026.
//

#include "Usuario.h"

Usuario::Usuario(std::string nom, int cod, bool car) : nombre(std::move(nom)), codigo(cod), carnet(car){}

std::string Usuario::getName() const {
    return nombre;
}
int Usuario::getCodigo() const {
    return codigo;
}
bool Usuario::hasCarnet() const {
    return carnet;
}

