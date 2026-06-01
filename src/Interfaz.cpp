#include "../include/Interfaz.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <vector>
#include <conio.h>
#include <cctype>
#include <string>
#include <random>
#include "SistemaTransporte.h"
#include "RutaBarrio.h"
#include "RutaCentro.h"
#include "Paradero.h"
#include "Horario.h"

// NOLINTBEGIN

Interfaz::Interfaz(SistemaTransporte& sis) : sistema(sis) {
    // Configuración para tildes y caracteres latinos en Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

Interfaz::~Interfaz() = default;

void Interfaz::registrarLogConsulta(const std::string& detalle) {
    std::ofstream archivoLog("log_consultas.txt", std::ios::app);
    if (!archivoLog.is_open()) return;
    const std::time_t tiempoActual = std::time(nullptr);
    const std::tm* tmInfo = std::localtime(&tiempoActual);
    archivoLog << std::put_time(tmInfo, "%Y-%m-%d %H:%M:%S") << " | " << detalle << "\n";
}

int Interfaz::tiempoAMinutos(int hora, int minutos) {
    return hora * 60 + minutos;
}

int Interfaz::obtenerMinutosActuales() {
    const std::time_t tiempoActual = std::time(nullptr);
    const std::tm* tmInfo = std::localtime(&tiempoActual);
    return tmInfo->tm_hour * 60 + tmInfo->tm_min;
}

// --- Visual Helpers (Blindados contra errores de fuente) ---

void Interfaz::setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Interfaz::dibujarLinea(char c, int ancho) {
    // Dibuja una línea con el carácter especificado, con tabulación para alinear
    std::cout << "\t";
    for (int i = 0; i < ancho; ++i) std::cout << c;
    std::cout << "\n";
}

void Interfaz::dibujarRecuadro(const std::string& titulo) {
    setColor(11); // Cyan
    std::cout << "\n\t+------------------------------------------------------------+\n";

    // Centrar el título
    int pos = (60 - static_cast<int>(titulo.length())) / 2;
    std::cout << "\t| ";
    for(int i=0; i<pos; ++i) std::cout << " ";
    std::cout << titulo;
    for(int i=0; i<(60-pos-static_cast<int>(titulo.length())); ++i) std::cout << " ";
    std::cout << " |\n";

    std::cout << "\t+------------------------------------------------------------+\n";
    setColor(7); // Blanco
}

void Interfaz::cerrarRecuadro() {
    setColor(11);
    std::cout << "\t+------------------------------------------------------------+\n";
    setColor(7);
}

void Interfaz::imprimirSugerencia(const std::string& texto) {
    setColor(14); // Amarillo
    std::cout << "\t  * SUGERENCIA: " << texto << "\n";
    setColor(7);
}

// --- Pantallas ---

void Interfaz::mostrarBienvenida() const {
    // Activar UTF-8 en la consola para mostrar tildes y caracteres especiales
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Intentar habilitar VT (ANSI) para mejorar compatibilidad de colores
    DWORD mode = 0;
    if (hConsole != INVALID_HANDLE_VALUE && GetConsoleMode(hConsole, &mode)) {
        SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    system("cls");

    const std::string title = "¡TU RUTA TUI!";
    const std::string subtitle = "SISTEMA DE RUTAS UNILLANOS (LUN-VIE)";
    const int width = 60;

    setColor(11);
    std::cout << "\n\n\n";
    dibujarLinea('=', width);
    int pad1 = (width - static_cast<int>(title.size())) / 2; if (pad1 < 0) pad1 = 0;
    std::cout << "\t" << std::string(pad1, ' ') << title << "\n";
    int pad2 = (width - static_cast<int>(subtitle.size())) / 2; if (pad2 < 0) pad2 = 0;
    std::cout << "\t" << std::string(pad2, ' ') << subtitle << "\n";
    dibujarLinea('=', width);
    setColor(7);

    std::cout << "\t  Presione cualquier tecla para iniciar...\n\n";

    // Determinar posición superior derecha para el reloj
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    COORD relojPos{0,0};
    if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        relojPos.X = static_cast<SHORT>(csbi.dwSize.X > 20 ? csbi.dwSize.X - 20 : 0);
        relojPos.Y = 0; // top row
    }

    // Reloj en la esquina superior derecha, actualizando sólo el tiempo
    while (!_kbhit()) {
        actualizarReloj(relojPos.X, relojPos.Y);
        Sleep(1000);
    }
    _getch();
}

void Interfaz::actualizarReloj(int x, int y) const {
    if (hConsole == INVALID_HANDLE_VALUE) return;
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    COORD pos{static_cast<SHORT>(x), static_cast<SHORT>(y)};
    // Mover cursor
    SetConsoleCursorPosition(hConsole, pos);
    // Formatear hora
    char buf[32];
    const std::time_t t = std::time(nullptr);
    const std::tm* tm = std::localtime(&t);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
    std::cout << buf;
    // Rellenar si es necesario (evitar restos)
    std::cout << std::flush;
    // Restaurar cursor (place it after printed time)
    COORD after = pos; after.X = static_cast<SHORT>(pos.X + (SHORT)strlen(buf));
    SetConsoleCursorPosition(hConsole, after);
}

void Interfaz::mostrarPantalla2_TipoRuta() const {
    while (true) {
        system("cls");
        (void)dibujarRecuadro("SELECCIÓN DE TIPO DE RUTA");

        // imprimir cabecera y dejar espacio para el reloj
        std::cout << "\tHora: ";
        // capturar la posición del tiempo
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            COORD timePos = csbi.dwCursorPosition;
            // imprimir el menú estático
            std::cout << "\n\n    [1] RUTA CENTRO\n";
            std::cout << "\n    [2] RUTA DE BARRIO\n";
            std::cout << "\n    ------------------------------------------------------------\n";
            std::cout << "    [0] Volver al Inicio | [Q] Salir del Sistema\n";
            cerrarRecuadro();

            std::cout << "\n\t\t\t Seleccione una opción: ";
            std::cout.flush();

            // actualizar reloj en su posición hasta que usuario presione tecla
            while (!_kbhit()) {
                actualizarReloj(timePos.X, timePos.Y);
                Sleep(1000);
            }
            int key = _getch();
            switch (key) {
                case '1': mostrarPantalla3_SentidoCentro(); break;
                case '2': mostrarPantalla3_ListaBarrio(); break;
                case '0': return;
                case 'q': case 'Q': exit(0); break;
                default: break;
            }
        } else {
            // Fallback: comportamiento anterior sin update in-place
            std::cout << "\n\n    [1] RUTA CENTRO\n";
            std::cout << "\n    [2] RUTA DE BARRIO\n";
            std::cout << "\n    ------------------------------------------------------------\n";
            std::cout << "    [0] Volver al Inicio | [Q] Salir del Sistema\n";
            cerrarRecuadro();
            int key = _getch();
            switch (key) {
                case '1': mostrarPantalla3_SentidoCentro(); break;
                case '2': mostrarPantalla3_ListaBarrio(); break;
                case '0': return;
                case 'q': case 'Q': exit(0); break;
                default: break;
            }
        }
    }
}

void Interfaz::mostrarPantalla3_SentidoCentro() const {
    // Para la ruta centro ya no pedimos "Ida/Vuelta". Se muestra directamente
    // el detalle operativo de la ruta centro (paraderos, frecuencia y, si la
    // ruta está activa en este preciso momento, las ETAs/progreso/retardos).
    mostrarPantalla4_InfoRuta(sistema.ruta_centro());
}

void Interfaz::mostrarPantalla3_ListaBarrio() const {
    while (true) {
        system("cls");
        const auto& rutas = sistema.getRutas();
        if (rutas.empty()) {
            std::cout << "No hay rutas de barrio cargadas.\n";
            _getch(); return;
        }

        (void)dibujarRecuadro("SELECCIÓN DE SUBRUTA DE BARRIO");

        // imprimir cabecera y reservar posición para el reloj
        std::cout << "\tHora: ";
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        COORD timePos{0,0};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            timePos = csbi.dwCursorPosition;
        }
        std::cout << "\n\n";

        // Listar rutas numeradas, alineadas
        for (size_t i = 0; i < rutas.size(); ++i) {
            std::cout << "    [" << std::setw(2) << (i+1) << "] " << std::left << std::setw(40) << rutas[i]->getName();
            if (auto* rb = dynamic_cast<RutaBarrio*>(rutas[i])) {
                std::cout << " (" << rb->getSector() << ")";
            }
            std::cout << "\n";
        }

        std::cout << "\n    ------------------------------------------------------------\n";
        std::cout << "    [0] Volver al Menú Principal\n";
        cerrarRecuadro();

        // input prompt position
        std::cout << "\n\t\t\t Ingrese el número de la ruta: ";
        COORD inputPos{0,0};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            inputPos = csbi.dwCursorPosition;
        }

        // Leer input con actualización de reloj en sitio
        std::string buffer;
        int idx = -1;
        while (true) {
            // actualizar reloj en su posición
            actualizarReloj(timePos.X, timePos.Y);

            if (_kbhit()) {
                int ch = _getch();
                if (ch == 13) { // Enter
                    if (buffer.empty()) continue;
                    try { idx = std::stoi(buffer) - 1; }
                    catch(...) { buffer.clear();
                        SetConsoleCursorPosition(hConsole, inputPos);
                        std::cout << std::string(10, ' ');
                        continue; }
                    if (buffer == "0") return;
                    break;
                } else if (ch == 8) { // Backspace
                    if (!buffer.empty()) buffer.pop_back();
                } else if (ch >= '0' && ch <= '9') {
                    buffer.push_back(static_cast<char>(ch));
                }
                // redraw input buffer only
                if (hConsole != INVALID_HANDLE_VALUE) SetConsoleCursorPosition(hConsole, inputPos);
                std::cout << buffer << " ";
                std::cout.flush();
            }
            Sleep(200);
        }

        if (idx != -1 && idx < static_cast<int>(rutas.size())) {
            mostrarPantalla4_InfoRuta(rutas[idx]);
        }
    }
}

void Interfaz::mostrarPantalla4_InfoRuta(const Ruta* ruta) const {
    if (!ruta) return;
    system("cls");

    // Random generators used in esta función. Solo se definen UNA vez aquí
    static thread_local std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<double> eventProb(0.0, 1.0);
    static std::uniform_int_distribution<int> delayDist(1, 10);

    // Si es la ruta centro, no pedimos ida/vuelta: mostramos paraderos y frecuencia.
    if (const RutaCentro* rc = dynamic_cast<const RutaCentro*>(ruta)) {
        // Preparar pantalla estática
        dibujarRecuadro("DETALLE RUTA CENTRO");
        std::cout << "\n    RUTA:   " << std::left << std::setw(25) << ruta->getName() << "\n";
        std::cout << "    FRECUENCIA: " << rc->getFrecuencia() << " minutos\n";
        std::cout << "    ESTADO: ";
        if (ruta->isActive()) { setColor(10); std::cout << "[ OPERATIVO ]"; }
        else { setColor(12); std::cout << "[ FUERA DE SERVICIO ]"; }
        setColor(7);
        std::cout << "\n\n    PARADEROS ASIGNADOS:\n";

        const auto& pars = ruta->getParaderos();
        for (size_t i = 0; i < pars.size(); ++i) {
            std::cout << "       " << i+1 << ". " << pars[i]->getName() << "\n";
        }

        // Si la ruta centro no está activa, informar y mostrar hora estimada de la próxima salida
        const int frecuencia = rc->getFrecuencia();
        int ahora = obtenerMinutosActuales();
        if (!ruta->isActive()) {
            int minutosHastaSiguiente = (frecuencia - (ahora % frecuencia)) % frecuencia;
            if (minutosHastaSiguiente == 0) minutosHastaSiguiente = frecuencia;
            int ttotal = ahora + minutosHastaSiguiente;
            int hh = (ttotal / 60) % 24;
            int mm = ttotal % 60;
            char buft[6]; snprintf(buft, sizeof(buft), "%02d:%02d", hh, mm);
            std::cout << "\n    Ninguna ruta está activa en este momento.\n";
            std::cout << "    Hora estimada para que salga la próxima ruta: " << buft << " (en " << minutosHastaSiguiente << " min)\n";
            std::cout << "\n    Presione cualquier tecla para volver...\n";
            _getch();
            system("cls");
            return;
        }

        // Eventos de tráfico y ETAs (se muestran SÓLO si la ruta está activa "en este preciso minuto").
        // rng, eventProb y delayDist se definen una vez al inicio de la función.

        // Posición del reloj (superior derecha)
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        COORD relojPos{0,0};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            relojPos.X = static_cast<SHORT>(csbi.dwSize.X > 20 ? csbi.dwSize.X - 20 : 0);
            relojPos.Y = csbi.dwCursorPosition.Y;
        }
        actualizarReloj(relojPos.X, relojPos.Y);

        // Reservar espacio para ETAs
        COORD etaPos{0,0};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) etaPos = csbi.dwCursorPosition;
        int reserve = static_cast<int>(pars.size()) + 4;
        for (int i = 0; i < reserve; ++i) std::cout << "\n";

        bool etaShown = false;
        int lastMinute = -1;

        // Loop interactivo: actualiza solo el reloj y muestra ETAs si corresponde.
        while (true) {
            if (_kbhit()) break; // la tecla permitirá entrar al menú
            // actualizar reloj
            actualizarReloj(relojPos.X, relojPos.Y);

            // comprobar cambio de minuto
            int ahoraMin = obtenerMinutosActuales();
            if (ahoraMin != lastMinute) {
                lastMinute = ahoraMin;
                bool shouldShowETA = false;
                if (ruta->isActive()) {
                    shouldShowETA = (ahoraMin % frecuencia) == 0;
                }

                int width = (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) ? csbi.dwSize.X : 80;
                if (hConsole != INVALID_HANDLE_VALUE) SetConsoleCursorPosition(hConsole, etaPos);

                if (shouldShowETA && !etaShown) {
                    // calcular evento de tráfico en el momento de mostrar ETAs
                    bool trafficEvent = eventProb(rng) < 0.25;
                    int trafficDelay = trafficEvent ? delayDist(rng) : 0;
                    // Calcular ETAs: tiempo hasta primer paradero = TIEMPO_ENTRE_PARADEROS
                    for (size_t i = 0; i < pars.size(); ++i) {
                        int eta = static_cast<int>(i + 1) * TIEMPO_ENTRE_PARADEROS + (trafficEvent ? trafficDelay : 0);
                        std::cout << std::left << std::setw(width) << (std::string("       ") + pars[i]->getName() + " ETA: " + std::to_string(eta) + " min") << "\n";
                    }
                    if (trafficEvent) {
                        setColor(12);
                        std::cout << std::left << std::setw(width) << (std::string("\n    TRÁFICO: Evento detectado, posible retraso de ~") + std::to_string(trafficDelay) + " minutos.") << "\n";
                        setColor(7);
                    }
                    etaShown = true;
                } else if (!shouldShowETA && etaShown) {
                    // limpiar area reservada
                    if (hConsole != INVALID_HANDLE_VALUE) SetConsoleCursorPosition(hConsole, etaPos);
                    for (int i = 0; i < reserve; ++i) std::cout << std::string(width, ' ') << "\n";
                    etaShown = false;
                }
            }
            Sleep(1000);
        }

        // tecla presionada: abrir menu de opciones para esta ruta
        int k = _getch(); (void)k;
        system("cls");

        // Mostrar menú con opción de ver info de bus/conductor si la ruta está activa
        dibujarRecuadro("OPCIONES - RUTA CENTRO");
        std::cout << "\n    [0] Regresar a la pantalla de bienvenida\n";
        if (ruta->isActive()) std::cout << "    [1] Ver información de buses y conductores\n";
        cerrarRecuadro();
        std::cout << "\n    Seleccione una opción: ";
        int sel = _getch();
        if (sel == '0') {
            system("cls");
            mostrarBienvenida();
            return;
        }
        if (sel == '1' && ruta->isActive()) {
            // Mostrar información de buses asignados a esta ruta
            system("cls");
            dibujarRecuadro("INFORMACIÓN DE BUSES Y CONDUCTORES");
            const auto& buses = sistema.getBuses();
            bool any = false;
            for (auto* b : buses) {
                if (b->getRutaAsignada() == ruta) {
                    any = true;
                    std::cout << "\n    PLACA: " << b->getPlaca() << " | CAPACIDAD: " << b->getCapacidad()
                              << " | DISPONIBLE: " << (b->isDisponible() ? "SI" : "NO") << "\n";
                    std::cout << "    OBSERVACION: " << b->getObservacion() << "\n";
                    if (auto* c = b->getConductorAsignado()) {
                        std::cout << "    Conductor: " << c->getName() << " (ID: " << c->getId() << ")\n";
                        std::cout << "      Licencia: " << c->getLicencia() << " | Tel: " << c->getTelefono() << "\n";
                    } else {
                        std::cout << "    Conductor: NO ASIGNADO\n";
                    }
                    std::cout << "\n";
                }
            }
            if (!any) std::cout << "\n    No hay buses asignados a esta ruta.\n";
            cerrarRecuadro();
            std::cout << "\n    Presione cualquier tecla para volver...";
            _getch();
            system("cls");
            // volver a mostrar info de la ruta
            mostrarPantalla4_InfoRuta(ruta);
            return;
        }
        // cualquier otra tecla: volver al menú anterior
        system("cls");
        return;
    }

    // --- Ruta no centro (barrio u otras): mostrar detalle fijo y permitir menú ---
    {
        (void)dibujarRecuadro("DETALLE OPERATIVO DE LA RUTA");
        std::cout << "\n    RUTA:   " << std::left << std::setw(25) << ruta->getName() << "\n";

        std::cout << "    ESTADO: ";
        if (ruta->isActive()) { setColor(10); std::cout << "[ OPERATIVO ]"; }
        else { setColor(12); std::cout << "[ FUERA DE SERVICIO ]"; }
        setColor(7);
        std::cout << "\n";

        if (!ruta->isActive()) {
            std::cout << "\n";
            imprimirSugerencia("La ruta seleccionada no esta activa. Verifique rutas alternativas.");
            const auto& todas = sistema.getRutas();
            for (auto* r : todas) {
                if (r->isActive()) {
                    setColor(14);
                    std::cout << "      -> Ruta sugerida activa: " << r->getName() << "\n";
                    setColor(7);
                    break;
                }
            }
        }

        std::cout << "\n    PARADEROS ASIGNADOS:\n";
        const auto& pars = ruta->getParaderos();
        for (size_t i = 0; i < pars.size(); ++i) {
            std::cout << "       " << i+1 << ". " << pars[i]->getName() << "\n";
        }

        std::cout << "\n    PROGRAMACION DE HORARIOS:\n";
        int ahora = obtenerMinutosActuales();
        const auto* rb = dynamic_cast<const RutaBarrio*>(ruta);
        std::vector<const Horario*> horariosVec;
        if (rb) {
            const auto& horarios = rb->getHorarios();
            for (const auto* h : horarios) horariosVec.push_back(h);
        }

        auto printSchedules = [&](void) {
            if (horariosVec.empty()) {
                std::cout << "       Sin horarios programados.\n";
                return;
            }

            // Calcular próximo horario
            int minDiff = 1440; int nextIdx = -1;
            for (int i = 0; i < static_cast<int>(horariosVec.size()); ++i) {
                int tm = tiempoAMinutos(std::stoi(horariosVec[i]->getHora()), std::stoi(horariosVec[i]->getMinuto()));
                int diff = tm - ahora;
                if (diff < 0) diff += 1440;
                if (diff < minDiff) { minDiff = diff; nextIdx = i; }
            }

            // Encabezado de tabla
            std::cout << "\n       HORA   | ESTADO\n";
            std::cout << "       -------+---------------------------------------------\n";
            for (int i = 0; i < static_cast<int>(horariosVec.size()); ++i) {
                const auto* h = horariosVec[i];
                int tm = tiempoAMinutos(std::stoi(h->getHora()), std::stoi(h->getMinuto()));
                int hh = tm / 60;
                int mm = tm % 60;
                char buf[6]; snprintf(buf, sizeof(buf), "%02d:%02d", hh, mm);
                // resaltar próximo
                if (i == nextIdx) setColor(10);
                std::cout << "       " << buf << "   | " << h->getEstado();
                if (i == nextIdx) {
                    std::cout << "  <-- Próximo (en " << minDiff << " min)";
                    setColor(7);
                }
                std::cout << "\n";
            }
        };

        printSchedules();

        // Pre-calcular minutos de los horarios para comprobaciones rápidas
        std::vector<int> scheduleMins;
        for (const auto* h : horariosVec) {
            scheduleMins.push_back(tiempoAMinutos(std::stoi(h->getHora()), std::stoi(h->getMinuto())));
        }

        // Preparar display para ETAs
        CONSOLE_SCREEN_BUFFER_INFO csbi2{};
        COORD etaPos{0,0};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi2)) {
            etaPos = csbi2.dwCursorPosition; // justo despues de los horarios
        }
        int reserve = static_cast<int>(pars.size()) + 4;
        for (int i = 0; i < reserve; ++i) std::cout << "\n";

        bool etaShown = false;
        int lastMinute = -1;

        // Determinar posición del reloj y comenzar loop de actualizaciones
        COORD relojPos{0,0};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi2)) {
            relojPos.X = static_cast<SHORT>(csbi2.dwSize.X > 20 ? csbi2.dwSize.X - 20 : 0);
            relojPos.Y = csbi2.dwCursorPosition.Y;
        }

        // Loop interactivo: actualiza solo el reloj y muestra ETAs cuando la hora coincide con algún horario
        while (true) {
            if (_kbhit()) break;
            actualizarReloj(relojPos.X, relojPos.Y);

            int ahoraMinLocal = obtenerMinutosActuales();
            if (ahoraMinLocal != lastMinute) {
                lastMinute = ahoraMinLocal;
                bool shouldShowETA = false;
                if (ruta->isActive()) {
                    for (int tm : scheduleMins) if (tm == ahoraMinLocal) { shouldShowETA = true; break; }
                }

                int width = (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi2)) ? csbi2.dwSize.X : 80;
                if (hConsole != INVALID_HANDLE_VALUE) SetConsoleCursorPosition(hConsole, etaPos);

                if (shouldShowETA && !etaShown) {
                    bool trafficEvent = eventProb(rng) < 0.25;
                    int trafficDelay = trafficEvent ? delayDist(rng) : 0;
                    int minutosHastaSiguiente = TIEMPO_ENTRE_PARADEROS - (ahoraMinLocal % TIEMPO_ENTRE_PARADEROS);
                    if (minutosHastaSiguiente == TIEMPO_ENTRE_PARADEROS) minutosHastaSiguiente = 0;
                    for (size_t i = 0; i < pars.size(); ++i) {
                        int eta = minutosHastaSiguiente + static_cast<int>(i) * TIEMPO_ENTRE_PARADEROS + (trafficEvent ? trafficDelay : 0);
                        std::cout << std::left << std::setw(width) << (std::string("       ") + pars[i]->getName() + " ETA: " + std::to_string(eta) + " min") << "\n";
                    }
                    if (trafficEvent) {
                        setColor(12);
                        std::cout << std::left << std::setw(width) << (std::string("\n    TRÁFICO: Evento detectado, posible retraso de ~") + std::to_string(trafficDelay) + " minutos.") << "\n";
                        setColor(7);
                    }
                    etaShown = true;
                } else if (!shouldShowETA && etaShown) {
                    if (hConsole != INVALID_HANDLE_VALUE) SetConsoleCursorPosition(hConsole, etaPos);
                    for (int i = 0; i < reserve; ++i) std::cout << std::string(width, ' ') << "\n";
                    etaShown = false;
                }
            }
            Sleep(1000);
        }

        // tecla presionada: abrir menu para esta ruta
        int k = _getch(); (void)k;
        system("cls");

        dibujarRecuadro("OPCIONES - RUTA");
        std::cout << "\n    [0] Regresar a la pantalla de bienvenida\n";
        if (ruta->isActive()) std::cout << "    [1] Ver información de buses y conductores\n";
        cerrarRecuadro();
        std::cout << "\n    Seleccione una opción: ";
        int sel = _getch();
        if (sel == '0') {
            system("cls");
            mostrarBienvenida();
            return;
        }
        if (sel == '1' && ruta->isActive()) {
            system("cls");
            dibujarRecuadro("INFORMACIÓN DE BUSES Y CONDUCTORES");
            const auto& buses = sistema.getBuses();
            bool any = false;
            for (auto* b : buses) {
                if (b->getRutaAsignada() == ruta) {
                    any = true;
                    std::cout << "\n    PLACA: " << b->getPlaca() << " | CAPACIDAD: " << b->getCapacidad()
                              << " | DISPONIBLE: " << (b->isDisponible() ? "SI" : "NO") << "\n";
                    std::cout << "    OBSERVACION: " << b->getObservacion() << "\n";
                    if (auto* c = b->getConductorAsignado()) {
                        std::cout << "    Conductor: " << c->getName() << " (ID: " << c->getId() << ")\n";
                        std::cout << "      Licencia: " << c->getLicencia() << " | Tel: " << c->getTelefono() << "\n";
                    } else {
                        std::cout << "    Conductor: NO ASIGNADO\n";
                    }
                    std::cout << "\n";
                }
            }
            if (!any) std::cout << "\n    No hay buses asignados a esta ruta.\n";
            cerrarRecuadro();
            std::cout << "\n    Presione cualquier tecla para volver...";
            _getch();
            system("cls");
            mostrarPantalla4_InfoRuta(ruta);
            return;
        }
        system("cls");
        return;
    }
}

        // NOLINTEND

