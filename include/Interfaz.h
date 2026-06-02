//
// Created by arman on 29/05/2026.
//

#ifndef PROYECTO_INTERFAZ_H
#define PROYECTO_INTERFAZ_H

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

class SistemaTransporte; //Forward Declaration
class Ruta; //Forward Declaration
class Paradero; //Forward Declaration
class Interfaz {
private:
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Control remoto para lograr cambiar cosas visuales de la consola
    int TIEMPO_ENTRE_PARADEROS = 6;
    SistemaTransporte& sistema;
    [[nodiscard]] static int tiempoAMinutos(int hora, int minutos); //Se le ingresa una hora y retorna ese tiempo en minutos que han pasado desde la medianoche
    [[nodiscard]] static int obtenerMinutosActuales(); //Obtiene los minutos actuales que han pasado desde la medianoche

    static void setColor(int color);
    static void dibujarLinea(char c, int ancho);
    static void dibujarRecuadro(const std::string& titulo);
    static void cerrarRecuadro();
    static void imprimirSugerencia(const std::string& texto);
    // Actualiza el reloj en la posición (x,y) sin redibujar toda la pantalla
    void actualizarReloj(int x, int y) const;
    static int calcularDemanda(int hora);
    void dibujarPanelDinamico(const std::vector<Paradero*>& paraderos,
                              int indiceActual, int totalParadas,
                              int minutosTranscurridos, int demanda,
                              bool& busToggle, int trafficDelay,
                              COORD startPos) const;


public:
    explicit Interfaz(SistemaTransporte& sis);
    ~Interfaz();

    static void registrarLogConsulta(const std::string& detalle);
    void mostrarBienvenida() const;
    void mostrarPantalla2_TipoRuta() const;
    void mostrarPantalla3_SentidoCentro() const;
    void mostrarPantalla3_ListaBarrio() const;
    void mostrarPantalla4_InfoRuta(const Ruta* ruta) const;
};


#endif //PROYECTO_INTERFAZ_H
