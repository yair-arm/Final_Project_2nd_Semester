//
// Created by arman on 12/05/2026.
//

#ifndef PROYECTO_RUTABARRIO_H
#define PROYECTO_RUTABARRIO_H
#include "Ruta.h"
#include "Horario.h"
#include <vector>


class RutaBarrio : public Ruta {
private:
    std::string sector;
    std::vector <Horario*> horarios;
public:
    RutaBarrio(std::string nombre, bool act, std::string descripcion, std::string sec);
    ~RutaBarrio() override = default;
    void agregarHorario(Horario* nuevoHorario); //Agregar nuevos horarios
    [[nodiscard]] const std::vector<Horario*>& getHorarios() const; //Metodo que devuelve el vector horarios de cada rutaBarrio
    [[nodiscard]] const std::string sector1() const;
};


#endif //PROYECTO_RUTABARRIO_H
