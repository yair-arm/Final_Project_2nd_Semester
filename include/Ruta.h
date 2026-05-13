#ifndef PROYECTO_RUTA_H        //Si no se ha definido antes, se usa la definición
#define PROYECTO_RUTA_H        //Definir la clase ruta

#include <iostream>
#include <string>

class Ruta {
protected:
    std::string nombre;
    std::string tipo;
    bool activa = true;
    std::string descripcion;
public:
    Ruta(const std::string& nombre, const std::string& tipo, bool act, const std::string& descripcion); //Constructor
    virtual ~Ruta(); //Destructor virtual
    void mostrarInformacion() const;
};


#endif //PROYECTO_RUTA_H
