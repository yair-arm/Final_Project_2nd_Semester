#include <iostream>
#include "RutaCentro.h"

int main() {
    std::cout << "Hello world!" << std::endl;
    const RutaCentro& centro = RutaCentro::getInstance();
    centro.mostrarInformacion();
    return 0;
}
