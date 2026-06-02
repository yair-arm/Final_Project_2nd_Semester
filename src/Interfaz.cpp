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

// --- Funciones auxiliares locales (no necesitan .h) ---
static int obtenerSegundosAhora() {
    const std::time_t t = std::time(nullptr);
    const std::tm* tm = std::localtime(&t);
    return tm->tm_hour * 3600 + tm->tm_min * 60 + tm->tm_sec;
}

static const char* nivelDemandaTexto(int d) {
    if (d >= 75) return "MUY ALTA";
    if (d >= 50) return "ALTA";
    if (d >= 25) return "MEDIA";
    return "BAJA";
}

static int retrasoPorDemanda(int demanda, int paradasRestantes) {
    return (demanda / 20) * paradasRestantes;
}

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

int Interfaz::calcularDemanda(int hora) {
    int base;
    if (hora >= 5 && hora <= 6)       base = 35;
    else if (hora >= 7 && hora <= 8)  base = 85;
    else if (hora >= 9 && hora <= 10) base = 60;
    else if (hora >= 11 && hora <= 13) base = 80;
    else if (hora >= 14 && hora <= 15) base = 50;
    else if (hora >= 16 && hora <= 18) base = 90;
    else if (hora >= 19 && hora <= 20) base = 60;
    else if (hora >= 21)               base = 25;
    else                               base = 20;

    int resultado = base + (rand() % 21 - 10);
    if (resultado < 0) resultado = 0;
    if (resultado > 100) resultado = 100;
    return resultado;
}

void Interfaz::dibujarPanelDinamico(
    const std::vector<Paradero*>& paraderos,
    const int indiceActual, const int totalParadas,
    const int minutosTranscurridos, const int demanda,
    bool& busToggle, const int trafficDelay,
    const COORD startPos
) const {
    if (hConsole == INVALID_HANDLE_VALUE) return;
    SetConsoleCursorPosition(hConsole, startPos);

    const int BAR_W = 30;
    const int totalTripMinutes = totalParadas * TIEMPO_ENTRE_PARADEROS;
    const float progreso = (totalTripMinutes > 0)
        ? std::min(1.0f, static_cast<float>(minutosTranscurridos) / static_cast<float>(totalTripMinutes))
        : 0.0f;
    const int filled = std::min(BAR_W - 1, static_cast<int>(progreso * BAR_W));
    busToggle = !busToggle;

    setColor(11);
    std::cout << "\t" << char(218) << char(196) << char(196) << " MAPA DE RUTA ";
    for (int i = 0; i < 37; ++i) std::cout << char(196);
    std::cout << char(191) << "\n";
    setColor(7);

    std::cout << "\t" << char(179) << "\n";

    std::cout << "\t" << char(179) << " ";
    for (int i = 0; i < BAR_W; ++i) {
        if (i < filled) {
            setColor(10);
            std::cout << char(219);
        } else if (i == filled) {
            setColor(14);
            std::cout << (busToggle ? char(219) : char(177));
            setColor(7);
        } else {
            setColor(8);
            std::cout << char(176);
        }
    }
    setColor(7);
    std::cout << " " << std::setw(3) << static_cast<int>(progreso * 100) << "%\n";

    std::cout << "\t" << char(179) << "\n";

    std::cout << "\t" << char(179) << " PARADEROS:" << std::string(28, ' ') << "ESTADO     ETA\n";

    for (int i = 0; i < totalParadas; ++i) {
        std::cout << "\t" << char(179) << "  " << std::setw(2) << (i+1) << ". "
                  << std::left << std::setw(34) << paraderos[i]->getName();
        if (i < indiceActual) {
            setColor(8);
            std::cout << char(251) << " PASADO";
        } else if (i == indiceActual) {
            setColor(14);
            std::cout << char(219) << " AHORA  " << char(24);
        } else {
            int paradasRest = i - indiceActual;
            int etaBase = paradasRest * TIEMPO_ENTRE_PARADEROS;
            int etaDem = retrasoPorDemanda(demanda, paradasRest);
            int etaTotal = etaBase + etaDem;
            if (etaTotal < 0) etaTotal = 0;
            setColor(7);
            std::cout << char(254) << " SIG.  en " << std::setw(2) << etaTotal << " min";
        }
        setColor(7);
        std::cout << "\n";
    }

    std::cout << "\t" << char(179) << "\n";

    std::cout << "\t" << char(179) << " DEMANDA: ";
    const int demBars = demanda / 10;
    int demColor = (demanda >= 75) ? 12 : (demanda >= 50) ? 14 : (demanda >= 25) ? 14 : 10;
    setColor(demColor);
    for (int i = 0; i < 10; ++i)
        std::cout << (i < demBars ? char(219) : char(176));
    setColor(7);
    std::cout << " " << std::setw(3) << demanda << "% (" << nivelDemandaTexto(demanda) << ")\n";

    int retrasoDem = retrasoPorDemanda(demanda, 1);
    setColor(demColor);
    std::cout << "\t" << char(179) << "    └─ Demanda " << nivelDemandaTexto(demanda)
              << ": +" << retrasoDem << " min/paradero\n";
    setColor(7);

    if (trafficDelay > 0 || demanda >= 50) {
        setColor(12);
        std::cout << "\t" << char(179) << " TRÁFICO: +" << (trafficDelay > 0 ? trafficDelay : retrasoDem)
                  << " min por demanda en hora "
                  << ((demanda >= 50) ? "pico" : "valle") << "\n";
        setColor(7);
    }

    setColor(11);
    std::cout << "\t" << char(192);
    for (int i = 0; i < 49; ++i) std::cout << char(196);
    std::cout << char(217) << "\n";
    setColor(7);
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
    registrarLogConsulta("INGRESO: Usuario inicio el sistema");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    DWORD mode = 0;
    if (hConsole != INVALID_HANDLE_VALUE && GetConsoleMode(hConsole, &mode)) {
        SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    system("cls");

    const int W = 55;
    const std::string titleBlock = "RUTA TUI";

    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    COORD relojPos{0,0};
    if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        relojPos.X = static_cast<SHORT>(csbi.dwSize.X > 22 ? csbi.dwSize.X - 22 : 0);
        relojPos.Y = 0;
    }

    std::cout << "\n";

    // ╔═ top ═╗
    setColor(11);
    std::cout << "\t" << char(201);
    for (int i = 0; i < W; ++i) std::cout << char(205);
    std::cout << char(187) << "\n";

    // ║       ║
    std::cout << "\t" << char(186) << std::string(W, ' ') << char(186) << "\n";

    // ║   ████  RUTA TUI  ████   ║
    std::cout << "\t" << char(186);
    {
        const std::string content = std::string(4, char(219)) + "  " + titleBlock + "  " + std::string(4, char(219));
        const int pad = (W - static_cast<int>(content.size())) / 2;
        setColor(14);
        std::cout << std::string(pad, ' ') << content << std::string(W - pad - static_cast<int>(content.size()), ' ');
    }
    setColor(11);
    std::cout << char(186) << "\n";

    // ║       ║
    std::cout << "\t" << char(186) << std::string(W, ' ') << char(186) << "\n";

    // ║  SISTEMA DE RUTAS UNILLANOS  ║
    const std::string s1 = "SISTEMA DE RUTAS UNILLANOS";
    const std::string s2 = "Lunes a Viernes";
    setColor(7);
    auto printCentered = [&](const std::string& s) {
        std::cout << "\t" << char(186);
        const int p = (W - static_cast<int>(s.size())) / 2;
        std::cout << std::string(p, ' ') << s << std::string(W - p - static_cast<int>(s.size()), ' ') << char(186) << "\n";
    };
    printCentered(s1);
    printCentered(s2);

    // ║       ║
    std::cout << "\t" << char(186) << std::string(W, ' ') << char(186) << "\n";

    // ║  Presione cualquier tecla...  ║
    const std::string prompt = "Presione cualquier tecla para iniciar";
    setColor(14);
    printCentered(prompt);
    setColor(7);

    // ║       ║
    std::cout << "\t" << char(186) << std::string(W, ' ') << char(186) << "\n";

    // ╚═ bottom ═╝
    setColor(11);
    std::cout << "\t" << char(200);
    for (int i = 0; i < W; ++i) std::cout << char(205);
    std::cout << char(188) << "\n";
    setColor(7);

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
    registrarLogConsulta("NAV: Seleccion de tipo de ruta");
    while (true) {
        system("cls");

        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        COORD relojPos{0,0};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            relojPos.X = static_cast<SHORT>(csbi.dwSize.X > 22 ? csbi.dwSize.X - 22 : 0);
            relojPos.Y = 0;
        }

        std::cout << "\n";
        (void)dibujarRecuadro("SELECCION DE TIPO DE RUTA");

        std::cout << "\n\n    [1] RUTA CENTRO\n";
        std::cout << "\n    [2] RUTA DE BARRIO\n";
        std::cout << "\n    ------------------------------------------------------------\n";
        std::cout << "    [0] Volver al Inicio | [Q] Salir del Sistema\n";
        cerrarRecuadro();

        std::cout << "\n\t\t\t Seleccione una opcion: ";
        std::cout.flush();

        while (!_kbhit()) {
            actualizarReloj(relojPos.X, relojPos.Y);
            Sleep(1000);
        }
        const int key = _getch();
        switch (key) {
            case '1': mostrarPantalla3_SentidoCentro(); break;
            case '2': mostrarPantalla3_ListaBarrio(); break;
            case '0': return;
            case 'q': case 'Q': exit(0); break;
            default: break;
        }
    }
}

void Interfaz::mostrarPantalla3_SentidoCentro() const {
    registrarLogConsulta("NAV: Consulto ruta centro");
    mostrarPantalla4_InfoRuta(sistema.ruta_centro());
}

void Interfaz::mostrarPantalla3_ListaBarrio() const {
    registrarLogConsulta("NAV: Explorando rutas de barrio");
    while (true) {
        system("cls");

        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        COORD relojPos{0,0};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            relojPos.X = static_cast<SHORT>(csbi.dwSize.X > 22 ? csbi.dwSize.X - 22 : 0);
            relojPos.Y = 0;
        }

        std::cout << "\n";

        const auto& rutas = sistema.getRutas();
        if (rutas.empty()) {
            std::cout << "\tNo hay rutas de barrio cargadas.\n";
            _getch(); return;
        }

        (void)dibujarRecuadro("SELECCION DE SUBRUTA DE BARRIO");

        for (size_t i = 0; i < rutas.size(); ++i) {
            std::cout << "    [" << std::setw(2) << (i+1) << "] " << std::left << std::setw(40) << rutas[i]->getName();
            if (auto* rb = dynamic_cast<RutaBarrio*>(rutas[i])) {
                std::cout << " (" << rb->getSector() << ")";
            }
            std::cout << "\n";
        }

        std::cout << "\n    ------------------------------------------------------------\n";
        std::cout << "    [0] Volver al Menu Principal\n";
        cerrarRecuadro();

        std::cout << "\n\t\t\t Ingrese el numero de la ruta: ";
        COORD inputPos{0,0};
        if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            inputPos = csbi.dwCursorPosition;
        }

        std::string buffer;
        int idx = -1;
        while (true) {
            actualizarReloj(relojPos.X, relojPos.Y);

            if (_kbhit()) {
                int ch = _getch();
                if (ch == 13) {
                    if (buffer.empty()) continue;
                    try { idx = std::stoi(buffer) - 1; }
                    catch(...) { buffer.clear();
                        SetConsoleCursorPosition(hConsole, inputPos);
                        std::cout << std::string(10, ' ');
                        continue; }
                    if (buffer == "0") return;
                    break;
                } else if (ch == 8) {
                    if (!buffer.empty()) buffer.pop_back();
                } else if (ch >= '0' && ch <= '9') {
                    buffer.push_back(static_cast<char>(ch));
                }
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
    registrarLogConsulta("CONSULTA: Detalle de ruta " + ruta->getName());
    system("cls");

    const auto& allPars = ruta->getParaderos();
    const auto* rc = dynamic_cast<const RutaCentro*>(ruta);
    const auto* rb = dynamic_cast<const RutaBarrio*>(ruta);

    int totalParadas = 0;
    std::vector<Paradero*> dirPars;

    if (rc) {
        dibujarRecuadro("SELECCIONE DIRECCION");
        std::cout << "\n    [1] Parque de Los Estudiantes  -->  Universidad de los Llanos\n";
        std::cout << "    [2] Universidad de los Llanos  -->  Parque de Los Estudiantes\n";
        cerrarRecuadro();
        std::cout << "\n    Opcion: ";
        const int ch = _getch();
        const int direccion = (ch == '2') ? 2 : 1;

        if (direccion == 2) {
            for (int i = 6; i >= 0; --i) dirPars.push_back(allPars[i]);
        } else {
            for (int i = 0; i < 7; ++i) dirPars.push_back(allPars[i]);
        }
        totalParadas = 7;
        system("cls");
    } else {
        dirPars = allPars;
        totalParadas = static_cast<int>(dirPars.size());
    }

    // --- HEADER estático ---
    {
        dibujarRecuadro(rc ? "DETALLE RUTA CENTRO" : "DETALLE RUTA DE BARRIO");
        std::cout << "\n    RUTA:   " << std::left << std::setw(25) << ruta->getName();
        if (rc) {
            const int hi = rc->getHoraInicio(), mi = rc->getMinInicio();
            const int hf = rc->getHoraFin(), mf = rc->getMinFin();
            std::cout << "HORARIO: "
                      << (hi < 10 ? "0" : "") << hi << ":" << (mi < 10 ? "0" : "") << mi
                      << "  -  "
                      << (hf < 10 ? "0" : "") << hf << ":" << (mf < 10 ? "0" : "") << mf
                      << "   |   FRECUENCIA: Cada " << rc->getFrecuencia() << " min\n";
        } else if (rb) {
            std::cout << "SECTOR: " << rb->getSector() << "\n";
        }

        std::cout << "    ESTADO: ";
        if (ruta->isActive()) { setColor(10); std::cout << "[ OPERATIVO ]"; }
        else { setColor(12); std::cout << "[ FUERA DE SERVICIO ]"; }
        setColor(7);
        std::cout << "\n";
    }

    if (!ruta->isActive()) {
        std::cout << "\n";
        imprimirSugerencia("La ruta seleccionada no esta activa. Verifique rutas alternativas.");
        for (auto* r : sistema.getRutas()) {
            if (r->isActive()) {
                setColor(14);
                std::cout << "      -> Ruta sugerida activa: " << r->getName() << "\n";
                setColor(7);
                break;
            }
        }
        std::cout << "\n    Presione cualquier tecla para volver...\n";
        _getch(); system("cls"); return;
    }

    // --- Calcular estado activo ---
    int ahora = obtenerMinutosActuales();
    int salidaActivaMinutos = 0;
    int minutosTranscurridos = 0;
    int indiceParadaActual = 0;
    bool hayBusActivo = false;

    if (rc) {
        const int freq = rc->getFrecuencia();
        const int inicio = rc->getHoraInicio() * 60 + rc->getMinInicio();
        const int fin = rc->getHoraFin() * 60 + rc->getMinFin();

        if (ahora >= inicio && ahora <= fin) {
            salidaActivaMinutos = inicio + ((ahora - inicio) / freq) * freq;
            if (salidaActivaMinutos > fin) salidaActivaMinutos = fin;
            if (salidaActivaMinutos >= inicio) {
                minutosTranscurridos = ahora - salidaActivaMinutos;
                const int tTR = totalParadas * TIEMPO_ENTRE_PARADEROS;
                if (minutosTranscurridos < tTR) {
                    hayBusActivo = true;
                    indiceParadaActual = minutosTranscurridos / TIEMPO_ENTRE_PARADEROS;
                    if (indiceParadaActual >= totalParadas) indiceParadaActual = totalParadas - 1;
                }
            }
        }
        if (hayBusActivo) {
            const int hh = (salidaActivaMinutos / 60) % 24, mm = salidaActivaMinutos % 60;
            std::cout << "\n    SALIDA: " << (hh < 10 ? "0" : "") << hh << ":"
                      << (mm < 10 ? "0" : "") << mm << "\n";
        }
    } else if (rb) {
        const auto& horarios = rb->getHorarios();
        const int tiempoTotalRuta = totalParadas * TIEMPO_ENTRE_PARADEROS;
        std::cout << "\n\n    PROGRAMACION DE HORARIOS:\n";
        std::cout << "       HORA   | ESTADO\n";
        std::cout << "       -------+---------------------------------------------\n";

        int minDiff = 1440, nextIdx = -1;
        for (int i = 0; i < static_cast<int>(horarios.size()); ++i) {
            const int tm = tiempoAMinutos(std::stoi(horarios[i]->getHora()), std::stoi(horarios[i]->getMinuto()));
            int diff = tm - ahora;
            if (diff < 0) diff += 1440;
            if (diff < minDiff) { minDiff = diff; nextIdx = i; }
        }

        for (int i = 0; i < static_cast<int>(horarios.size()); ++i) {
            const int tm = tiempoAMinutos(std::stoi(horarios[i]->getHora()), std::stoi(horarios[i]->getMinuto()));
            const int hh = tm / 60, mm = tm % 60;
            char buf[6]; snprintf(buf, sizeof(buf), "%02d:%02d", hh, mm);
            if (i == nextIdx) setColor(10);
            std::cout << "       " << buf << "   | " << horarios[i]->getEstado();
            if (i == nextIdx) { std::cout << "  <-- Proximo (en " << minDiff << " min)"; setColor(7); }
            std::cout << "\n";

            if (ruta->isActive() && ahora >= tm && ahora < tm + tiempoTotalRuta) {
                hayBusActivo = true;
                salidaActivaMinutos = tm;
                minutosTranscurridos = ahora - tm;
                indiceParadaActual = minutosTranscurridos / TIEMPO_ENTRE_PARADEROS;
                if (indiceParadaActual >= totalParadas) indiceParadaActual = totalParadas - 1;
            }
        }

        if (hayBusActivo) {
            setColor(10);
            const int hh = salidaActivaMinutos / 60, mm = salidaActivaMinutos % 60;
            std::cout << "\n    BUS EN RUTA desde las "
                      << (hh < 10 ? "0" : "") << hh << ":"
                      << (mm < 10 ? "0" : "") << mm << "\n";
            setColor(7);
        }
    }

    std::cout << "\n";

    // --- Posiciones fijas ---
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    COORD dynamicPos{0,0};
    COORD relojPos{0,0};
    int reserve = totalParadas + 13;

    if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        dynamicPos = csbi.dwCursorPosition;
        relojPos.X = static_cast<SHORT>(csbi.dwSize.X > 22 ? csbi.dwSize.X - 22 : 0);
        relojPos.Y = 0;
    }
    for (int i = 0; i < reserve; ++i) std::cout << "\n";

    // --- Loop dinámico: actualiza segundo a segundo ---
    int lastMinute = -1;
    int trafficDelay = 0;
    int demanda = calcularDemanda(ahora / 60);

    while (true) {
        if (_kbhit()) break;
        actualizarReloj(relojPos.X, relojPos.Y);

        int ahoraMin = obtenerMinutosActuales();
        int ahoraSeg = obtenerSegundosAhora();

        // Recalcular demanda y tráfico cada minuto
        if (ahoraMin != lastMinute) {
            lastMinute = ahoraMin;
            demanda = calcularDemanda(ahoraMin / 60);

            if (hayBusActivo) {
                int retrasoBase = retrasoPorDemanda(demanda, 1);
                trafficDelay = retrasoBase + (rand() % 3 - 1);
                if (trafficDelay < 0) trafficDelay = 0;
            }
        }

        if (hayBusActivo) {
            if (rc) {
                const int freq = rc->getFrecuencia();
                const int inicio = rc->getHoraInicio() * 60 + rc->getMinInicio();
                const int fin = rc->getHoraFin() * 60 + rc->getMinFin();
                if (ahoraMin >= inicio && ahoraMin <= fin) {
                    salidaActivaMinutos = inicio + ((ahoraMin - inicio) / freq) * freq;
                    if (salidaActivaMinutos > fin) { hayBusActivo = false; }
                    else {
                        int salidaSeg = salidaActivaMinutos * 60;
                        int segundosTranscurridos = ahoraSeg - salidaSeg;
                        if (segundosTranscurridos < 0) segundosTranscurridos = 0;
                        minutosTranscurridos = segundosTranscurridos / 60;
                        int tTR = totalParadas * TIEMPO_ENTRE_PARADEROS;
                        if (minutosTranscurridos < tTR) {
                            indiceParadaActual = minutosTranscurridos / TIEMPO_ENTRE_PARADEROS;
                            if (indiceParadaActual >= totalParadas) indiceParadaActual = totalParadas - 1;

                            if (hConsole != INVALID_HANDLE_VALUE) {
                    SetConsoleCursorPosition(hConsole, dynamicPos);
                    std::cout << "\x1B[J" << std::flush;
                    CONSOLE_SCREEN_BUFFER_INFO clr{};
                    DWORD written2 = 0;
                    if (GetConsoleScreenBufferInfo(hConsole, &clr)) {
                        const DWORD cellCount = static_cast<DWORD>(reserve) * static_cast<DWORD>(clr.dwSize.X);
                        FillConsoleOutputCharacterA(hConsole, ' ', cellCount, dynamicPos, &written2);
                        FillConsoleOutputAttribute(hConsole, clr.wAttributes, cellCount, dynamicPos, &written2);
                    }
                    SetConsoleCursorPosition(hConsole, dynamicPos);
                }

                            const int BAR_W = 30;
                            const int totalTripMinutes = totalParadas * TIEMPO_ENTRE_PARADEROS;
                            const int totalTripSeconds = totalTripMinutes * 60;
                            const float progreso = (totalTripSeconds > 0)
                                ? std::min(1.0f, static_cast<float>(segundosTranscurridos) / static_cast<float>(totalTripSeconds))
                                : 0.0f;
                            const int filled = std::min(BAR_W - 1, static_cast<int>(progreso * BAR_W));

                            static bool blink = false;
                            blink = !blink;

                            setColor(11);
                            std::cout << "\t" << char(218) << char(196) << char(196) << " MAPA DE RUTA ";
                            for (int i = 0; i < 37; ++i) std::cout << char(196);
                            std::cout << char(191) << "\n";
                            setColor(7);

                            std::cout << "\t" << char(179) << "\n";

                            std::cout << "\t" << char(179) << " ";
                            for (int i = 0; i < BAR_W; ++i) {
                                if (i < filled) {
                                    setColor(10);
                                    std::cout << char(219);
                                } else if (i == filled) {
                                    setColor(14);
                                    std::cout << (blink ? char(219) : char(177));
                                    setColor(7);
                                } else {
                                    setColor(8);
                                    std::cout << char(176);
                                }
                            }
                            setColor(7);
                            std::cout << " " << std::setw(3) << static_cast<int>(progreso * 100) << "%\n";

                            std::cout << "\t" << char(179) << "\n";
                            std::cout << "\t" << char(179) << " VIAJE: "
                                      << (segundosTranscurridos / 60) << "m "
                                      << (segundosTranscurridos % 60) << "s\n";
                            std::cout << "\t" << char(179) << " PARADEROS:" << std::string(28, ' ') << "ESTADO     ETA\n";

                            for (int i = 0; i < totalParadas; ++i) {
                                std::cout << "\t" << char(179) << "  " << std::setw(2) << (i+1) << ". "
                                          << std::left << std::setw(34) << dirPars[i]->getName();
                                if (i < indiceParadaActual) {
                                    setColor(8);
                                    std::cout << char(251) << " PASADO";
                                } else if (i == indiceParadaActual) {
                                    setColor(14);
                                    std::cout << char(219) << " AHORA  " << char(24);
                                } else {
                                    int paradasRest = i - indiceParadaActual;
                                    int segEnSeg = segundosTranscurridos % (TIEMPO_ENTRE_PARADEROS * 60);
                                    int etaBase = paradasRest * TIEMPO_ENTRE_PARADEROS - (segEnSeg / 60);
                                    if (etaBase < 0) etaBase = 0;
                                    int etaDem = retrasoPorDemanda(demanda, paradasRest);
                                    int etaTotal = etaBase + etaDem;
                                    if (etaTotal < 0) etaTotal = 0;
                                    setColor(7);
                                    std::cout << char(254) << " SIG.  en " << std::setw(2) << etaTotal << " min";
                                }
                                setColor(7);
                                std::cout << "\n";
                            }

                            std::cout << "\t" << char(179) << "\n";
                            std::cout << "\t" << char(179) << " DEMANDA: ";
                            const int demBars = demanda / 10;
                            int demColor = (demanda >= 75) ? 12 : (demanda >= 50) ? 14 : (demanda >= 25) ? 14 : 10;
                            setColor(demColor);
                            for (int i = 0; i < 10; ++i)
                                std::cout << (i < demBars ? char(219) : char(176));
                            setColor(7);
                            std::cout << " " << std::setw(3) << demanda << "% (" << nivelDemandaTexto(demanda) << ")\n";

                            int retrasoXParada = retrasoPorDemanda(demanda, 1);
                            setColor(demColor);
                            std::cout << "\t" << char(179) << "    └─ Demanda " << nivelDemandaTexto(demanda)
                                      << ": +" << retrasoXParada << " min/paradero\n";
                            setColor(7);

                            if (trafficDelay > 0 || demanda >= 50) {
                                setColor(12);
                                std::cout << "\t" << char(179) << " TRÁFICO: +" << (trafficDelay > 0 ? trafficDelay : retrasoXParada)
                                          << " min por demanda en hora "
                                          << ((demanda >= 50) ? "pico" : "valle") << "\n";
                                setColor(7);
                            }

                            setColor(11);
                            std::cout << "\t" << char(192);
                            for (int i = 0; i < 49; ++i) std::cout << char(196);
                            std::cout << char(217) << "\n";
                            setColor(7);
                            std::cout << std::flush;
                        } else {
                            hayBusActivo = false;
                        }
                    }
                } else {
                    hayBusActivo = false;
                }
            } else {
                int salidaSeg = salidaActivaMinutos * 60;
                int segundosTranscurridos = ahoraSeg - salidaSeg;
                if (segundosTranscurridos < 0) segundosTranscurridos = 0;
                minutosTranscurridos = segundosTranscurridos / 60;
                indiceParadaActual = minutosTranscurridos / TIEMPO_ENTRE_PARADEROS;
                if (indiceParadaActual >= totalParadas || minutosTranscurridos >= totalParadas * TIEMPO_ENTRE_PARADEROS) {
                    hayBusActivo = false;
                } else {
                    if (hConsole != INVALID_HANDLE_VALUE) {
                    SetConsoleCursorPosition(hConsole, dynamicPos);
                    std::cout << "\x1B[J" << std::flush;
                    CONSOLE_SCREEN_BUFFER_INFO clr{};
                    DWORD written2 = 0;
                    if (GetConsoleScreenBufferInfo(hConsole, &clr)) {
                        const DWORD cellCount = static_cast<DWORD>(reserve) * static_cast<DWORD>(clr.dwSize.X);
                        FillConsoleOutputCharacterA(hConsole, ' ', cellCount, dynamicPos, &written2);
                        FillConsoleOutputAttribute(hConsole, clr.wAttributes, cellCount, dynamicPos, &written2);
                    }
                    SetConsoleCursorPosition(hConsole, dynamicPos);
                }

                    const int BAR_W = 30;
                    const int totalTripMinutes = totalParadas * TIEMPO_ENTRE_PARADEROS;
                    const int totalTripSeconds = totalTripMinutes * 60;
                    const float progreso = (totalTripSeconds > 0)
                        ? std::min(1.0f, static_cast<float>(segundosTranscurridos) / static_cast<float>(totalTripSeconds))
                        : 0.0f;
                    const int filled = std::min(BAR_W - 1, static_cast<int>(progreso * BAR_W));

                    static bool blink = false;
                    blink = !blink;

                    setColor(11);
                    std::cout << "\t" << char(218) << char(196) << char(196) << " MAPA DE RUTA ";
                    for (int i = 0; i < 37; ++i) std::cout << char(196);
                    std::cout << char(191) << "\n";
                    setColor(7);

                    std::cout << "\t" << char(179) << "\n";

                    std::cout << "\t" << char(179) << " ";
                    for (int i = 0; i < BAR_W; ++i) {
                        if (i < filled) {
                            setColor(10);
                            std::cout << char(219);
                        } else if (i == filled) {
                            setColor(14);
                            std::cout << (blink ? char(219) : char(177));
                            setColor(7);
                        } else {
                            setColor(8);
                            std::cout << char(176);
                        }
                    }
                    setColor(7);
                    std::cout << " " << std::setw(3) << static_cast<int>(progreso * 100) << "%\n";

                    std::cout << "\t" << char(179) << "\n";
                    std::cout << "\t" << char(179) << " VIAJE: "
                              << (segundosTranscurridos / 60) << "m "
                              << (segundosTranscurridos % 60) << "s\n";
                    std::cout << "\t" << char(179) << " PARADEROS:" << std::string(28, ' ') << "ESTADO     ETA\n";

                    for (int i = 0; i < totalParadas; ++i) {
                        std::cout << "\t" << char(179) << "  " << std::setw(2) << (i+1) << ". "
                                  << std::left << std::setw(34) << dirPars[i]->getName();
                        if (i < indiceParadaActual) {
                            setColor(8);
                            std::cout << char(251) << " PASADO";
                        } else if (i == indiceParadaActual) {
                            setColor(14);
                            std::cout << char(219) << " AHORA  " << char(24);
                        } else {
                            int paradasRest = i - indiceParadaActual;
                            int segEnSeg = segundosTranscurridos % (TIEMPO_ENTRE_PARADEROS * 60);
                            int etaBase = paradasRest * TIEMPO_ENTRE_PARADEROS - (segEnSeg / 60);
                            if (etaBase < 0) etaBase = 0;
                            int etaDem = retrasoPorDemanda(demanda, paradasRest);
                            int etaTotal = etaBase + etaDem;
                            if (etaTotal < 0) etaTotal = 0;
                            setColor(7);
                            std::cout << char(254) << " SIG.  en " << std::setw(2) << etaTotal << " min";
                        }
                        setColor(7);
                        std::cout << "\n";
                    }

                    std::cout << "\t" << char(179) << "\n";
                    std::cout << "\t" << char(179) << " DEMANDA: ";
                    const int demBars = demanda / 10;
                    int demColor = (demanda >= 75) ? 12 : (demanda >= 50) ? 14 : (demanda >= 25) ? 14 : 10;
                    setColor(demColor);
                    for (int i = 0; i < 10; ++i)
                        std::cout << (i < demBars ? char(219) : char(176));
                    setColor(7);
                    std::cout << " " << std::setw(3) << demanda << "% (" << nivelDemandaTexto(demanda) << ")\n";

                    int retrasoXParada = retrasoPorDemanda(demanda, 1);
                    setColor(demColor);
                    std::cout << "\t" << char(179) << "    └─ Demanda " << nivelDemandaTexto(demanda)
                              << ": +" << retrasoXParada << " min/paradero\n";
                    setColor(7);

                    if (trafficDelay > 0 || demanda >= 50) {
                        setColor(12);
                        std::cout << "\t" << char(179) << " TRÁFICO: +" << (trafficDelay > 0 ? trafficDelay : retrasoXParada)
                                  << " min por demanda en hora "
                                  << ((demanda >= 50) ? "pico" : "valle") << "\n";
                        setColor(7);
                    }

                    setColor(11);
                    std::cout << "\t" << char(192);
                    for (int i = 0; i < 49; ++i) std::cout << char(196);
                    std::cout << char(217) << "\n";
                    setColor(7);
                    std::cout << std::flush;
                }
            }
        } else {
            if (hConsole != INVALID_HANDLE_VALUE) {
                    SetConsoleCursorPosition(hConsole, dynamicPos);
                    std::cout << "\x1B[J" << std::flush;
                    CONSOLE_SCREEN_BUFFER_INFO clr{};
                    DWORD written2 = 0;
                    if (GetConsoleScreenBufferInfo(hConsole, &clr)) {
                        const DWORD cellCount = static_cast<DWORD>(reserve) * static_cast<DWORD>(clr.dwSize.X);
                        FillConsoleOutputCharacterA(hConsole, ' ', cellCount, dynamicPos, &written2);
                        FillConsoleOutputAttribute(hConsole, clr.wAttributes, cellCount, dynamicPos, &written2);
                    }
                    SetConsoleCursorPosition(hConsole, dynamicPos);
                }
            int proxSalida = 0, proxDiff = 0;
            if (rc) {
                const int freq = rc->getFrecuencia();
                const int inicio = rc->getHoraInicio() * 60 + rc->getMinInicio();
                const int fin = rc->getHoraFin() * 60 + rc->getMinFin();
                if (ahoraMin < inicio) {
                    proxSalida = inicio; proxDiff = inicio - ahoraMin;
                } else if (ahoraMin >= fin) {
                    proxSalida = inicio + 1440; proxDiff = inicio + 1440 - ahoraMin;
                } else {
                    proxSalida = inicio + ((ahoraMin - inicio) / freq + 1) * freq;
                    if (proxSalida > fin) proxSalida = fin;
                    proxDiff = proxSalida - ahoraMin;
                }
            } else if (rb) {
                int minD = 1440;
                for (const auto* h : rb->getHorarios()) {
                    const int tm = tiempoAMinutos(std::stoi(h->getHora()), std::stoi(h->getMinuto()));
                    int d = tm - ahoraMin;
                    if (d < 0) d += 1440;
                    if (d < minD) { minD = d; proxSalida = tm; proxDiff = d; }
                }
            }
            setColor(14);
            std::cout << "\n\t  No hay buses en ruta en este momento.\n";
            const int hh = (proxSalida / 60) % 24, mm = proxSalida % 60;
            std::cout << "\t  Proxima salida: "
                      << (hh < 10 ? "0" : "") << hh << ":"
                      << (mm < 10 ? "0" : "") << mm
                      << " (en " << proxDiff << " min)\n";
            setColor(7);
            std::cout << std::flush;
        }

        Sleep(1000);
    }

    registrarLogConsulta("MENU: Usuario salio del modo en vivo de " + ruta->getName());
    int k = _getch(); (void)k;
    system("cls");

    dibujarRecuadro("OPCIONES");
    std::cout << "\n    [1] Ver informacion de buses y conductores\n";
    std::cout << "    [0] Volver al menu principal\n";
    std::cout << "    [Q] Salir del sistema\n";
    cerrarRecuadro();
    std::cout << "\n    Seleccione una opcion: ";

    const int sel = _getch();
    switch (sel) {
        case '0':
            system("cls");
            return;
        case 'q': case 'Q':
            exit(0);
        case '1':
            if (!ruta->isActive()) break;
            registrarLogConsulta("CONSULTA: Buses de ruta " + ruta->getName());
            system("cls");
            dibujarRecuadro("INFORMACION DE BUSES Y CONDUCTORES");
            {
                bool any = false;
                for (auto* b : sistema.getBuses()) {
                    if (b->getRutaAsignada() != ruta) continue;
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
                if (!any) std::cout << "\n    No hay buses asignados a esta ruta.\n";
            }
            cerrarRecuadro();
            std::cout << "\n    Presione cualquier tecla para volver...";
            _getch();
            system("cls");
            mostrarPantalla4_InfoRuta(ruta);
            return;
    }
    system("cls");
}

        // NOLINTEND

