//
// Created by arman on 12/05/2026.
//

#include "RutaBarrio.h"

RutaBarrio::RutaBarrio(const std::string &nombre, bool act, const std::string &descripcion, std::string  bar)
: Ruta(nombre, "De barrio", act, descripcion), barrio(std::move(bar)){}
