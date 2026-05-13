#include <iostream>
#include "Ruta.h"
#include "RutaCentro.h"

int main() {
    std::cout << "Hello world!" << std::endl;
    RutaCentro& centro = RutaCentro::getInstance();
    centro.mostrarInformacion();
    return 0;
}
