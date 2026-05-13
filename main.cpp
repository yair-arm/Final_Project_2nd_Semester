#include <iostream>
#include "include/Ruta.h"

int main() {
    std::cout << "Hello world!" << std::endl;
    Ruta ruta1("Centro", "Centro", true, "Ruta normal de centro");
    ruta1.mostrarInformacion();
    return 0;
}