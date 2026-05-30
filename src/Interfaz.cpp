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

void Interfaz::registrarLogConsulta(const std::string& detalle) const { //El const al final significa qye este metodo es de solo "lectura" para la clase Interfa.
    //Es decir, registrar un log no modifica ninguna variable interna de la interfaz.

    std::ofstream archivoLog("log_consultas.txt", std::ios::app);
    //El std::ios::app es para que el archivo se abra en modo append para que si el archivo se abre, el nuevo texto se agregue al final del archivo

    //Validación de seguridad por si falla el sistema de archivos
    if (!archivoLog.is_open()) {
        throw IncidenciaExcepcion("ARCHIVO NO ENCONTRADO", "No se ha podido abrir el archivo ");
    }
    const std::time_t tiempoActual = std::time(nullptr);
    const std:: tm* tmInfo = std::localtime(&tiempoActual);
    archivoLog << std::put_time(tmInfo, "%Y-%m-%d %H:%M:%S") << " | " << detalle << "/n"; //Aqui no es std::cout sino archivoLog
    //pues el out envia la info directamente a la pantalla, consola o terminal, archivoLog, es que un std::ofstream envia la informacion a un archivo de texto que en este caso es log_consultas.txt
}




void Interfaz::mostrarBienvenida() const {

}



