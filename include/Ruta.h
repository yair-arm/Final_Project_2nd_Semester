#ifndef PROYECTO_RUTA_H        //Si no se ha definido antes, se usa la definición
#define PROYECTO_RUTA_H        //Definir la clase ruta

#include <string>
#include <vector>
#include "Paradero.h"

class Ruta {
protected:
    std::vector <Paradero*> paraderos;
    std::string nombre;
    std::string tipo;
    bool activa = true;
    std::string descripcion;
public:
    Ruta(std::string nombre, std::string tipo, bool act, std::string descripcion); //Constructor
    virtual ~Ruta(); //Destructor virtual
    void agregarParadero(Paradero* nuevoParadero); //sistemaTransporte usa el constructor de paradero para crear un nuevo objeto y guardarlo en el vector

    [[nodiscard]] std::vector<Paradero *> paraderos1() const;
    [[nodiscard]] std::string nombre1() const;
    [[nodiscard]] std::string tipo1() const;
    [[nodiscard]] bool activa1() const;
    [[nodiscard]] std::string descripcion1() const;
};

#endif //PROYECTO_RUTA_H
