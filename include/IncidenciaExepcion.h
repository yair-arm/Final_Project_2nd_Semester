//
// Created by Nicolle on 5/26/2026.
//

#ifndef PROYECTO_INCIDENCIAEXCEPCION_H
#define PROYECTO_INCIDENCIAEXCEPCION_H

#include <exception>
#include <string>

class IncidenciaExcepcion : public std::exception {
private:
    std::string condicion;
    std::string mensaje;

public:
    // Constructor
    IncidenciaExcepcion(std::string condicion, std::string mensaje);

    //'override' al destructor para sobrescribir el de std::exception de forma segura
    ~IncidenciaExcepcion() override;

    // Método para tu reporte personalizado
    [[nodiscard]] std::string infoExcepcion() const;

    // El "noexcept" promete al compilador que esta función no fallará
    [[nodiscard]] const char* what() const noexcept override;
};

#endif //PROYECTO_INCIDENCIAEXCEPCION_H