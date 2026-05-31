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
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Control remoto para lograr cambiar cosas visuales de la consola
    int TIEMPO_ENTRE_PARADEROS = 6;
    SistemaTransporte& sistema;

    // Herramientas matemáticas independeientes (Sí pueden ser static)
    [[nodiscard]]  static int tiempoAMinutos(int hora, int minutos); //Se le ingresa una hora y retorna ese tiempo en minutos que han pasado desde la medianoche
    [[nodiscard]]  static int obtenerMinutosActuales(); //Obtiene los minutos actuales que han pasado desde la medianoche

public:
    explicit Interfaz(SistemaTransporte& sis);
    ~Interfaz();

    // El log puede ser static si no usa variables de la clase
    static void registrarLogConsulta(const std::string& detalle);

    // Estas NO deben ser static porque usan hConsole y el objeto sistema
    void mostrarBienvenida();
    void mostrarPantalla2_TipoRuta();
    void mostrarPantalla3_SentidoCentro();
    void mostrarPantalla4_InfoCentro(const Ruta* ruta);
    void mostrarPantalla5_ListaBarrio();
    void mostrarPantalla6_InfoBarrio(const Ruta* ruta);
};


#endif //PROYECTO_INTERFAZ_H
