//
// Created by arman on 12/05/2026.
//

#ifndef PROYECTO_RUTACENTRO_H
#define PROYECTO_RUTACENTRO_H
#include "Ruta.h"


class RutaCentro : public Ruta {
private:
    RutaCentro();
    int frecuencia = 30;
    int horaInicio = 5;
    int minInicio = 20;
    int horaFin = 19;
    int minFin = 10;
public:
    static RutaCentro& getInstance();

    RutaCentro(const RutaCentro&) = delete;
    void operator=(const RutaCentro&) = delete;
    ~RutaCentro() override = default;

    [[nodiscard]] int getFrecuencia() const;
    [[nodiscard]] int getHoraInicio() const;
    [[nodiscard]] int getMinInicio() const;
    [[nodiscard]] int getHoraFin() const;
    [[nodiscard]] int getMinFin() const;
};

#endif //PROYECTO_RUTACENTRO_H
