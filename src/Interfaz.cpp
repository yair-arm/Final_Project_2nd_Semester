//
// Created by arman on 29/05/2026.
//

#include "../include/Interfaz.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include "IncidenciaExepcion.h"
#include "SistemaTransporte.h"
#include <conio.h> //Para _kbhit el detectar "cualquier techa" windows,se usa para saber si presionaron algo sin bloquear el loop.

Interfaz::Interfaz(SistemaTransporte& sis) : sistema(sis) {}
Interfaz::~Interfaz() = default;

void Interfaz::registrarLogConsulta(const std::string& detalle) { //El const al final significa qye este metodo es de solo "lectura" para la clase Interfa.
    //Es decir, registrar un log no modifica ninguna variable interna de la interfaz.

    std::ofstream archivoLog("log_consultas.txt", std::ios::app);
    //El std::ios::app es para que el archivo se abra en modo append para que si el archivo se abre, el nuevo texto se agregue al final del archivo

    //Validación de seguridad por si falla el sistema de archivos
    if (!archivoLog.is_open()) {
        throw IncidenciaExcepcion("ARCHIVO NO ENCONTRADO", "No se ha podido abrir el archivo ");
    }
    const std::time_t tiempoActual = std::time(nullptr); //Obtiene la hora actual en segundos
    const std:: tm* tmInfo = std::localtime(&tiempoActual); //Convierte esos segundos enuna estructura std::tm con el año, mes, dia, hora.. adaptada a la zona horaria local
    archivoLog << std::put_time(tmInfo, "%Y-%m-%d %H:%M:%S") << " | " << detalle << "\n"; //Aqui no es std::cout sino archivoLog
    //pues el out envia la info directamente a la pantalla, consola o terminal, archivoLog, es que un std::ofstream envia la informacion a un archivo de texto que en este caso es log_consultas.txt
}

int Interfaz::tiempoAMinutos(const int hora, const int minuto) {
    return hora*60 + minuto;
}

int Interfaz::obtenerMinutosActuales() const {
    const std::time_t tiempoActual = std::time(nullptr); //numero gigante
    const std::tm* tmInfo = std::localtime(&tiempoActual);
    return tmInfo->tm_hour*60 + tmInfo->tm_min; //tmInfo->tm-hour accede a la hora actual en formato de 0-23
    // tm_hour y tm_min vienen de std::tm de la libreria <ctime>
}


void Interfaz::mostrarBienvenida() {
    system("cls");
    SetConsoleTextAttribute(hConsole, 11); // 11 = cyan
    std::cout << "Tu Ruta TUI";
    SetConsoleTextAttribute(hConsole, 7); // 7 = blanco
    std::cout << "Presione cualquier tecla para continuar...\n\n";

        while (!_kbhit()) {
             const std::time_t t = std::time(nullptr);
             const std::tm* tm = std::localtime(&t);

            std::cout << "SISTEMA DE RUTAS UNILLANOS [LUN-VIE] RELOJ: " << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
            std::cout.flush(); //Para forzar que se muestre inmediatamente

            Sleep(1000); //esperar 1 segundo (windows.h)
        }
}



