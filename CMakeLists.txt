cmake_minimum_required(VERSION 3.22)
project(Proyecto)

set(CMAKE_CXX_STANDARD 20)

# 1. Configurar la búsqueda de tus archivos de cabecera locales
include_directories(include)

# 2. Descargar e instalar la librería nlohmann/json automáticamente desde GitHub
include(FetchContent)
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

# 3. Definir tu ejecutable con todos tus archivos .cpp y .h
add_executable(Proyecto main.cpp
        src/Ruta.cpp
        include/Ruta.h
        src/RutaCentro.cpp
        include/RutaCentro.h
        src/RutaBarrio.cpp
        include/RutaBarrio.h
        include/Horario.h
        src/Horario.cpp
        src/Paradero.cpp
        include/Paradero.h
        src/Bus.cpp
        include/Bus.h
        src/Conductor.cpp
        include/Conductor.h
        src/Usuario.cpp
        include/Usuario.h
        include/SistemaTransporte.h
        src/IncidenciaExepcion.cpp
        include/IncidenciaExepcion.h
)

# 4. Vincular la librería JSON descargada con tu proyecto ejecutable
target_link_libraries(Proyecto PRIVATE nlohmann_json::nlohmann_json)

# 5. Copiar tu carpeta de datos (JSONs) al directorio de compilación automáticamente
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})