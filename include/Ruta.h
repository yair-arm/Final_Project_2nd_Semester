#ifndef PROYECTO_RUTA_H        //Si no se ha definido antes, se usa la definición
#define PROYECTO_RUTA_H        //Definir la clase ruta

#include <string>

class Ruta {
protected:
    std::string nombre;
    std::string tipo;
    bool activa = true;
    std::string descripcion;
public:
    Ruta(std::string nombre, std::string tipo, bool act, std::string descripcion); //Constructor
    virtual ~Ruta(); //Destructor virtual
    virtual void mostrarInformacion() const = 0;
};

#endif //PROYECTO_RUTA_H
