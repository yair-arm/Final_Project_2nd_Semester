//
// Created by arman on 21/05/2026.
//

#ifndef PROYECTO_CONDUCTOR_H
#define PROYECTO_CONDUCTOR_H
#include <string>

//Se debe incluir ambas clases en los archivos cpp mas no en los archivos .h
class Bus; //forward declaration para evitar bucles de includes infinitos


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

    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getLicencia() const;
    [[nodiscard]] std::string getTelefono() const;
    [[nodiscard]] Bus* getBusAsignado() const;

    void asignarBus(Bus* nuevoBus);
    void desvincularBus();
};


#endif //PROYECTO_CONDUCTOR_H
