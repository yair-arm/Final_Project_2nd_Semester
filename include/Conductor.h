//
// Created by arman on 21/05/2026.
//

#ifndef PROYECTO_CONDUCTOR_H
#define PROYECTO_CONDUCTOR_H
#include <string>
#include "Bus.h"

class Conductor {
private:
    std::string idConductor;
    std::string nombre;
    std::string licencia;
    std::string telefono;
    Bus* busAsignado = nullptr;
public:
    Conductor (std::string idConductor, std::string nombre, std::string licencia, std::string telefono);
    ~Conductor() = default;

    [[nodiscard]] std::string id_conductor() const;
    [[nodiscard]] std::string nombre1() const;
    [[nodiscard]] std::string licencia1() const;
    [[nodiscard]] std::string telefono1() const;
    [[nodiscard]] Bus* busAsignado1() const;

    void asignarBus(Bus* nuevoBus);
    void desvincularBus();
};


#endif //PROYECTO_CONDUCTOR_H
