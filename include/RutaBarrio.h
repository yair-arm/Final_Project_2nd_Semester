//
// Created by arman on 12/05/2026.
//

#ifndef PROYECTO_RUTABARRIO_H
#define PROYECTO_RUTABARRIO_H
#include "Ruta.h"


class RutaBarrio : public Ruta {
private:
    std::string barrio;
public:
    RutaBarrio(const std::string &nombre, bool act, const std::string &descripcion, std::string  bar);
};


#endif //PROYECTO_RUTABARRIO_H
