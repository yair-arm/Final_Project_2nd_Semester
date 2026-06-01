//
// Created by arman on 20/05/2026.
//

#ifndef PROYECTO_HORARIO_H
#define PROYECTO_HORARIO_H

#include <string>
class Horario {
private:
    std::string hora;
    std::string minuto;
    std::string estado;
public:
    Horario(std::string hor, std::string min, std::string est);
    ~Horario();
    void actualizarEstado(std::string nuevoEstado);
    //Este metodo que nos permitirá actualizar el estado de la ruta de "A tiempo" a por ejemplo "Retrasada"
    [[nodiscard]] std::string getHora() const;
    [[nodiscard]] std::string getMinuto() const;
    [[nodiscard]] std::string getEstado() const;
};

#endif //PROYECTO_HORARIO_H
