//
// Created by arman on 12/05/2026.
//
#include "../include/RutaCentro.h"

// Definimos los valores fijos de la ruta centro
RutaCentro::RutaCentro()
    : Ruta("Ruta parque", "centro", true, "Ruta principal, ruta centro") {}

RutaCentro& RutaCentro::getInstance() {
    // 3. El truco: una variable estática local se crea solo una vez
    static RutaCentro instancia;
    return instancia;
}

int RutaCentro::getFrecuencia() const {
    return frecuencia;
}

