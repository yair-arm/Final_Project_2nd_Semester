//
// Created by Nicolle on 5/26/2026.
//

#ifndef PROYECTO_SISTEMATRANSPORTE_H
#define PROYECTO_SISTEMATRANSPORTE_H
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Bus.h"
#include "Ruta.h"
#include "Conductor.h"
#include "RutaCentro.h"
#include "RutaBarrio.h"

class SistemaTransporte {
private:
    std::vector<Ruta*> rutas;
    std::vector<Bus*> buses;
    SistemaTransporte();
    void cargarDatosDesdeArchivo();
    void procesarYCrearBuses(nlohmann::json jBuses);
    void procesarYCrearRutas(const nlohmann::json& jRutas);
public:
    static SistemaTransporte& getInstance();
    ~SistemaTransporte();
    SistemaTransporte(const SistemaTransporte&) = delete;
    void operator=(const SistemaTransporte&) = delete;
    void iniciarSistema();
    void generarReporteGeneral() const;
    [[nodiscard]] Bus* consultarBusPorPlaca(const std::string& placa) const;
    [[nodiscard]] Ruta* consultarRutaPorNombre(const std::string& nombreRuta) const;
};

#endif //PROYECTO_SISTEMATRANSPORTE_H