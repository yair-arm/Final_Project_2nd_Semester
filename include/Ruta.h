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
    // Sobrecarga del operador << para poder imprimir la ruta en la interfaz (por referencia)
    friend std::ostream &operator<<(std::ostream &os, const Ruta &ruta);
    // Operadores para orden y comparación (útiles para containers y tests)
    [[nodiscard]] bool operator<(const Ruta &other) const;
    [[nodiscard]] bool operator==(const Ruta &other) const;
    [[nodiscard]] const std::vector<Paradero*>& getParaderos() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getType() const;
    [[nodiscard]] bool isActive() const;
    [[nodiscard]] std::string getDescription() const;
};

#endif //PROYECTO_RUTA_H
