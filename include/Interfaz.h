//
// Created by arman on 29/05/2026.
//

#ifndef PROYECTO_INTERFAZ_H
#define PROYECTO_INTERFAZ_H

#include <windows.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <chrono>

class SistemaTransporte; //Forward Declaration
class Ruta; //Forward Declaration
class Interfaz {
private:
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int TIEMPO_ENTRE_PARADEROS = 6;
    SistemaTransporte& sistema;
    [[nodiscard]] int tiempoAMinutos(int hora, int minutos) const; //Se le ingresa una hora y retorna ese tiempo en minutos que han pasado desde la medianoche
    [[nodiscard]] int obtenerMinutosActuales() const; //Obtiene los minutos actuales que han pasado desde la medianoche
public:
    explicit Interfaz(SistemaTransporte& sis);
    ~Interfaz();
    static void registrarLogConsulta(const std::string& detalle);
    static void mostrarBienvenida();
    void mostrarPantalla2_TipoRuta() const;
    void mostrarPantalla3_SentidoCentro() const;
    void mostrarPantalla4_InfoCentro(const Ruta* ruta) const;
    void mostrarPantalla5_ListaBarrio() const;
    void mostrarPantalla6_InfoBarrio(const Ruta* ruta) const;
};


#endif //PROYECTO_INTERFAZ_H
