//
// Created by arman on 29/05/2026.
//

#include "../include/Interfaz.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include "IncidenciaExepcion.h"
#include "SistemaTransporte.h"

Interfaz::Interfaz(SistemaTransporte& sis) : sistema(sis) {}
Interfaz::~Interfaz() = default;

void Interfaz::registrarLogConsulta(const std::string& detalle) const {
    std::ofstream archivoLog("log_consultas.txt", std::ios::app);
    //El std::ios::app es para que el archivo se abra en modo append para que si el archivo se abra, el nuevo texto se agregue al final del archivo

    //Validación de seguridad por si falla el sistema de archivos
    if (!archivoLog.is_open()) {
        throw IncidenciaExcepcion("ARCHIVO NO ENCONTRADO", "No se ha podido abrir el archivo ");
    }
    std::time_t tiempoActual = std::time(nullptr);
}




void Interfaz::mostrarBienvenida() const {

}



