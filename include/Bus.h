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
    bool disponible = true;
    std::string observacion;
public:
    Bus(std::string placa, int _cap, Ruta* _rutaA, bool _disponible, std::string _obser);
    ~Bus();
    void asignarRuta(Ruta* nuevaRuta);
    void asignarConductor(Conductor* nuevoConductor);
    void desvincularRuta();
    void desvincularConductor();
    [[nodiscard]] std::string getPlaca() const;
    [[nodiscard]] bool isDisponible() const;
    [[nodiscard]] std::string getObservacion() const;
    [[nodiscard]] int getCapacidad() const;
    [[nodiscard]] Ruta* getRutaAsignada() const;
    [[nodiscard]] Conductor* getConductorAsignado() const;
};


#endif //PROYECTO_BUS_H
