//
// Created by arman on 12/05/2026.
//

#ifndef PROYECTO_RUTACENTRO_H
#define PROYECTO_RUTACENTRO_H
#include "Ruta.h"


class RutaCentro : public Ruta {
private:
    // 1. Constructor privado: nadie fuera de esta clase puede crear una instancia
    RutaCentro();
    int frecuencia = 30;
public:
    // 2. Metodo estatico para obtener la única instancia
    static RutaCentro& getInstance();

    // Evitar que se copie la instancia
    RutaCentro(const RutaCentro&) = delete;
    void operator=(const RutaCentro&) = delete;
    void mostrarInformacion() const override;
};

#endif //PROYECTO_RUTACENTRO_H
