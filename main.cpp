#include "../include/SistemaTransporte.h"
#include "../include/IncidenciaExepcion.h"
#include <conio.h>
#include <exception>
#include <iostream>

int main() {
    try {
        SistemaTransporte::getInstance().iniciarSistema();
        return 0;
    } catch (const IncidenciaExcepcion& e) {
        std::cerr << "\nERROR: " << e.infoExcepcion() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "\nERROR: " << e.what() << "\n";
    }

    std::cout << "\nPresione cualquier tecla para cerrar...";
    _getch();
    return 1;
}
