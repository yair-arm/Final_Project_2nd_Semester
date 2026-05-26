//
// Created by arman on 26/05/2026.
//

#ifndef PROYECTO_USUARIO_H
#define PROYECTO_USUARIO_H
#include <string>

class Usuario {
private:
    std::string nombre;
    int codigo = 0;
    bool carnet = true;
public:
    Usuario(std::string nom, int cod, bool car);

    [[nodiscard]] std::string nombre1() const;
    [[nodiscard]] int codigo1() const;
    [[nodiscard]] bool carnet1() const;
};


#endif //PROYECTO_USUARIO_H
