//
// Created by arman on 26/05/2026.
//

#include "Usuario.h"

Usuario::Usuario(std::string nom, int cod, bool car) : nombre(std::move(nom)), codigo(cod), carnet(car){}

std::string Usuario::nombre1() const {
    return nombre;
}
int Usuario::codigo1() const {
    return codigo;
}
bool Usuario::carnet1() const {
    return carnet;
}

