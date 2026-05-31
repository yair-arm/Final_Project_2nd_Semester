#include "include/SistemaTransporte.h"

int main() {
    // El Singleton carga los datos, crea la interfaz y la muestra
    SistemaTransporte::getInstance().iniciarSistema();
    return 0;
}
