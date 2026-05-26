//
// Created by Nicolle on 5/26/2026.
//

#ifndef PROYECTO_SISTEMATRANSPORTE_H
#define PROYECTO_SISTEMATRANSPORTE_H
#include <vector>
#include "Ruta.h"
#include "Bus.h"
#include <nlohmann/json>

class SistemaTransporte {
private:
    std::vector<Ruta*> rutas;
    std::vector<Bus*> buses;
    SistemaTransporte();
};



#endif //PROYECTO_SISTEMATRANSPORTE_H