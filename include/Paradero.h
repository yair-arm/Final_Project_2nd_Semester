//
// Created by arman on 21/05/2026.
//

#ifndef PROYECTO_PARADERO_H
#define PROYECTO_PARADERO_H
#include <string>

class Paradero {
private:
    std::string nombre;
    std::string ubicacion;
public:
    Paradero(std::string nom, std::string ubi);
    ~Paradero() = default;

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getUbicacion() const;
};


#endif //PROYECTO_PARADERO_H
