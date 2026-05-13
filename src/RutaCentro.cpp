//
// Created by arman on 12/05/2026.
//

#include <iostream>
#include "../include/RutaCentro.h"

// Definimos los valores fijos de la ruta centro
RutaCentro::RutaCentro()
    : Ruta("Centro", "Centro", true, "Ruta principal, ruta centro") {}

RutaCentro& RutaCentro::getInstance() {
    // 3. El truco: una variable estática local se crea solo una vez
    static RutaCentro instancia;
    return instancia;
}
void RutaCentro::mostrarInformacion() const {
    Ruta::mostrarInformacion();
    std::cout << "Frecuencia: " << frecuencia << " minutos" << std::endl;
    std::cout << "------------------------------" << std::endl;
}

