#include "../include/SistemaTransporte.h"
#include "../include/IncidenciaExepcion.h"
#include <conio.h>
#include <exception>
#include <iostream>

int main() {
    int codigoSalida = 0;

    try {
        SistemaTransporte::getInstance().iniciarSistema();
        std::cout << "\nPrograma finalizado. Presione cualquier tecla para cerrar...";
    } catch (const IncidenciaExcepcion& e) {
        codigoSalida = 1;
        std::cerr << "\nERROR: " << e.infoExcepcion() << "\n";
    } catch (const std::exception& e) {
        codigoSalida = 1;
        std::cerr << "\nERROR: " << e.what() << "\n";
    }

    if (codigoSalida != 0) {
        std::cout << "\nPresione cualquier tecla para cerrar...";
    }

    _getch();
    return codigoSalida;
}
