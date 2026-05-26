//
// Created by Nicolle on 5/26/2026.
//

#ifndef PROYECTO_SISTEMATRANSPORTE_H
#define PROYECTO_SISTEMATRANSPORTE_H
#include <vector>
#include "Ruta.h"
#include "Bus.h"
#include "Conductor.h"
#include <string>
#include <\cmake-build-debvug\_deps\json-src\include>
#include <fstream>

class SistemaTransporte {
private:
    std::vector<Ruta*> rutas;
    std::vector<Bus*> buses;
    SistemaTransporte();
    void cargarDatosDesdeArchivo();

public:
    static SistemaTransporte& getInstance(); //getter para el constructor y mediante ella crear la única instancia de la clase
    ~SistemaTransporte(); //el destructor no van con default pues su comportamiento debe definirse dentro del cpp, ya que este contiene punteros, vectores...

    void procesarCrearBusConductor(Bus* nuevoBus, Conductor* nuevoConductor);
    void registrarRuta(const nlohmann::json& jRuta);
    void cargarHorariosDesdeArchivo();
    void asignarRutaABus(std::string placa, std::string nombreRuta);
    void generarReporte();

};



#endif //PROYECTO_SISTEMATRANSPORTE_H