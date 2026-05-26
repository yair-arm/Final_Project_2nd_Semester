//
// Created by Nicolle on 5/26/2026.
//

#ifndef PROYECTO_INCIDENCIAEXCEPCION_H
#define PROYECTO_INCIDENCIAEXCEPCION_H
#include <exception>
#include <string>

class IncidenciaExcepcion : public std::exception{
private:
    std::string condicion;
    std::string mensaje;
public:
    IncidenciaExcepcion(std::string condicion, std::string mensaje);
    ~IncidenciaExcepcion();
    std::string infoExcepcion() const;

    //El "noexcept" promete al compilador que esta funcion no fallará
    const char* what() const noexcept override;
};


#endif //PROYECTO_INCIDENCIAEXCEPCION_H