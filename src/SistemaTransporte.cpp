//
// Created by Nicolle on 5/26/2026.
//

#include "../include/SistemaTransporte.h"
// #include "../include/Interfaz.h"
#include "../include/RutaBarrio.h"
#include "../include/RutaCentro.h"
#include "../include/Conductor.h"
#include "../include/Paradero.h"
#include "../include/Horario.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

SistemaTransporte::SistemaTransporte() : interfazGrafica(nullptr) {}
SistemaTransporte& SistemaTransporte::getInstance() {
    static SistemaTransporte instancia;
    return instancia;
}

SistemaTransporte::~SistemaTransporte() {
    // Destruir buses, al llamar estos destructores, automaticamente también se destruyen los conductores
    for (const auto* Bus : buses) {
        if (Bus != nullptr){
            delete Bus;
        }
    }
    buses.clear();

    // Destruir rutas, esto elimina los horarios para rutas de barrio porque así está específicado
    for (auto* Ruta: rutas) {
        if (dynamic_cast<RutaCentro*>(Ruta) == nullptr) {
            delete Ruta;
        }
    }
    rutas.clear();

    //delete interfazGrafica; //Comentado debido a que aun no existe una interfaz la cual destruir
    //interfazGrafica = nullptr;
}

const std::vector<Bus*>& SistemaTransporte::getBuses() const {
    return buses;
}

const std::vector<Ruta*>& SistemaTransporte::getRutas() const {
    return rutas;
}

Bus* SistemaTransporte::consultarBusPorPlaca(const std::string& placa) const {
    for (auto* bus : buses) {
        if (bus->placa1() == placa) {
            return bus;
        }
    }
    return nullptr;
}

Ruta* SistemaTransporte::consultarRutaPorNombre(const std::string& nombre) const {
    for (auto* ruta : rutas) {
        if (ruta->nombre1() == nombre) {
            return ruta;
        }
    }
    return nullptr;
}

//Inicialización del metodo complejo para crear nuevas rutas, usa polimorfimo y memoria dinámica para decidir
//en tiempo de ejecución qué estructura de creación de instancias seguir según las instrucciones establecidas anteriormente
//Además de parsear o mapear los campos de los json a los atributos correspondientes de las rutas, los horarios y los paraderos
//Pendiente su revisión
void SistemaTransporte::procesarYCrearRutas(const nlohmann::json& jRutas) {
    for (const auto& jRuta : jRutas) {
        const std::string tipo = jRuta["tipo"];
        const std::string nombre = jRuta["nombre"];
        const bool activa = jRuta["activa"];
        const std::string descripcion = jRuta["descripcion"];

        Ruta* ruta = nullptr;

        if (tipo == "centro") {
            ruta = &RutaCentro::getInstance();
        } else if (tipo == "barrio") {
            const std::string sector = jRuta["sector"];
            ruta = new RutaBarrio(nombre, activa, descripcion, sector);
        }

        if (ruta == nullptr) continue;

        for (const auto& jParadero : jRuta["paraderos"]) {
            const std::string nomParadero = jParadero.get<std::string>();
            const auto p = new Paradero(nomParadero, "");
            ruta->agregarParadero(p);
        }

        if (tipo == "barrio") {
            const auto rutaBarrio = dynamic_cast<RutaBarrio*>(ruta);
            for (const auto& jHorario : jRuta["horarios"]) {
                const std::string hora   = jHorario["hora"];
                const std::string minuto = jHorario["minuto"];
                const std::string estado = jHorario["estado"];
                const auto h = new Horario(hora, minuto, estado);
                rutaBarrio->agregarHorario(h);
            }
        }

        rutas.push_back(ruta);
    }
}
