//
// Created by Nicolle on 5/26/2026.
//

#ifndef PROYECTO_SISTEMATRANSPORTE_H
#define PROYECTO_SISTEMATRANSPORTE_H

#include <vector>
#include "Ruta.h"
#include "Bus.h"
#include "RutaCentro.h"
#include "nlohmann/json.hpp"
#include <string>

class Interfaz;

class SistemaTransporte {
private:
    std::vector<Ruta*> rutas;
    std::vector<Bus*> buses;
    Interfaz* interfazGrafica;
    RutaCentro* rutaCentro = nullptr; //Se inicializa un puntero de tipo RutaCentro porque debe ser creado antes de
    //SistemaTransporte evitar una doble destrucción y memory leak

    const std::string ARCHIVO_BUSES = "data/buses_conductores.json";
    const std::string ARCHIVO_RUTAS = "data/rutas_paraderos_horarios.json";

    SistemaTransporte();
    void cargarDatosDesdeArchivo();
    void procesarYCrearRutas(const nlohmann::json& jRutas);
    void procesarYCrearBuses(const nlohmann::json& jBuses);

public:
    SistemaTransporte(const SistemaTransporte&) = delete;
    void operator=(const SistemaTransporte&) = delete;

    static SistemaTransporte& getInstance();
    ~SistemaTransporte();
    void iniciarSistema();

    [[nodiscard]] const std::vector<Bus*>& getBuses() const;
    [[nodiscard]] const std::vector<Ruta*>& getRutas() const;
    [[nodiscard]] Bus* consultarBusPorPlaca(const std::string& placa) const;
    [[nodiscard]] Ruta* consultarRutaPorNombre(const std::string& nombre) const;
    [[nodiscard]] RutaCentro * ruta_centro() const;
};


#endif //PROYECTO_SISTEMATRANSPORTE_H