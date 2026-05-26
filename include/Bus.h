//
// Created by arman on 21/05/2026.
//

#ifndef PROYECTO_BUS_H
#define PROYECTO_BUS_H
#include <string>
#include "Conductor.h"
#include "Ruta.h"

class Bus {
private:
    std::string placa;
    int capacidad = 0;
    Ruta* rutaAsignada = nullptr;
    Conductor* conductorAsignado = nullptr;
public:
    Bus(std::string placa, int _cap, Ruta* _rutaA);
    ~Bus() = default;
    void asignarRuta(Ruta* nuevaRuta);
    void asignarConductor(Conductor* nuevoConductor);
    void desvincularRuta();
    void desvincularConductor();
    [[nodiscard]] std::string placa1() const;
    [[nodiscard]] int capacidad1() const;
    [[nodiscard]] Ruta* ruta_asignada() const;
    [[nodiscard]] Conductor* conductor_asignado() const;
};


#endif //PROYECTO_BUS_H
