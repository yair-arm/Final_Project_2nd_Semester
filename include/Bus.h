//
// Created by arman on 21/05/2026.
//

#ifndef PROYECTO_BUS_H
#define PROYECTO_BUS_H
#include <string>
#include "Ruta.h"

class Bus {
private:
    std::string placa;
    int capacidad = 0;
    Ruta* rutaAsignada = nullptr;
public:
    Bus(std::string placa, int _cap, Ruta* _rutaA);
    ~Bus() = default;
    void asignarRuta(Ruta* nuevaRuta);
    void desvincularRuta();
    [[nodiscard]] std::string placa1() const;
    [[nodiscard]] int capacidad1() const;
    [[nodiscard]] Ruta * ruta_asignada() const;
};


#endif //PROYECTO_BUS_H
