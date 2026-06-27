# 🚌 Sistema de Gestión de Transporte Urbano

Simulación de un sistema de transporte público hecho en C++ como proyecto final de Programación Orientada a Objetos.

## Qué hace

El programa carga rutas de buses, paraderos, horarios, conductores y buses desde archivos JSON, y presenta una interfaz de consola donde podés:

- Ver las rutas disponibles (centro y barrios)
- Consultar paraderos de cada ruta
- Ver horarios con estado actualizado (a tiempo/retrasado)
- Consultar buses asignados a cada ruta con su conductor
- Hacer seguimiento en vivo de una ruta con simulación de paradas
- Registrar consultas en un archivo de log

## Cómo está armado

```
Proyecto/
├── main.cpp
├── include/
│   ├── SistemaTransporte.h    ← singleton, orquesta todo
│   ├── Interfaz.h             ← toda la UI de consola
│   ├── Ruta.h                 ← clase base abstracta
│   ├── RutaCentro.h           ← hereda de Ruta (singleton)
│   ├── RutaBarrio.h           ← hereda de Ruta, tiene horarios
│   ├── Bus.h / Conductor.h    ← composición
│   ├── Paradero.h / Horario.h ← objetos anidados
│   ├── Usuario.h              ← consultante del sistema
│   └── IncidenciaExcepcion.h  ← excepciones personalizadas
├── src/                       ← implementaciones
├── data/
│   ├── rutas_paraderos_horarios.json
│   └── buses_conductores.json
└── CMakeLists.txt
```

## Cosas que aprendí/build

- **Herencia y polimorfismo:** `RutaCentro` y `RutaBarrio` heredan de `Ruta`. Una es singleton (la ruta del centro es única), la otra tiene un vector de horarios porque cada ruta de barrio maneja sus propios turnos.
- **Singleton:** `SistemaTransporte` y `RutaCentro` usan el patrón singleton con instancia estática y constructor privado.
- **Sobrecarga de operadores:** `Ruta` sobrecarga `<<` para imprimirse directo en consola, y los operadores `<` y `==` para comparar rutas.
- **Archivos JSON:** Todo el dataset (rutas, paraderos, horarios, conductores, buses) se carga desde archivos JSON externos usando `nlohmann/json`. El sistema resuelve la ruta del ejecutable en Windows para encontrar los archivos.
- **Excepciones personalizadas:** `IncidenciaExcepcion` extiende `std::exception` con información extra para reportar errores del sistema (archivos faltantes, rutas inválidas, etc.).
- **Interfaz de consola con Windows API:** La interfaz usa `windows.h` para colores en la terminal, dibujar recuadros, mostrar un reloj en vivo, animar un indicador de bus en movimiento, y calcular demanda de pasajeros según la hora del día (horas pico vs valle).
- **Log de consultas:** Cada vez que alguien consulta una ruta, queda registrado en `log_consultas.txt` con timestamp.

## Cómo compilarlo

Requiere CMake y un compilador C++17. El proyecto está pensado para Windows (usa `<windows.h>` y `<conio.h>`).

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

El binario se genera en `cmake-build-debug/`. Al ejecutarlo, busca la carpeta `data/` relativa a la ruta del ejecutable.

## Créditos

Yair Armando Ospino Herrera — Segundo Semestre, Programación Orientada a Objetos, Corte III.
