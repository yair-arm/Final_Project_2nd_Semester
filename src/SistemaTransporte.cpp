#include "../include/SistemaTransporte.h"
#include "../include/Interfaz.h"
#include "../include/RutaBarrio.h"
#include "../include/RutaCentro.h"
#include "../include/Conductor.h"
#include "../include/Paradero.h"
#include "../include/Horario.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <windows.h>
#include "IncidenciaExcepcion.h"

namespace {
    std::string construirRutaDatos(const std::string& nombreArchivo) {
        char rutaEjecutable[MAX_PATH];
        const DWORD longitud = GetModuleFileNameA(nullptr, rutaEjecutable, MAX_PATH);

        if (longitud == 0 || longitud == MAX_PATH) {
            throw IncidenciaExcepcion("RUTA DEL EJECUTABLE NO DISPONIBLE",
                                      "No se pudo obtener la ruta del ejecutable actual");
        }

        const std::string rutaCompleta(rutaEjecutable);
        const size_t posicionSeparador = rutaCompleta.find_last_of("\\/");

        if (posicionSeparador == std::string::npos) {
            throw IncidenciaExcepcion("RUTA DEL EJECUTABLE INVALIDA",
                                      "No se pudo determinar la carpeta del ejecutable actual");
        }

        const std::string carpetaEjecutable = rutaCompleta.substr(0, posicionSeparador + 1);
        return carpetaEjecutable + "data\\" + nombreArchivo;
    }
}

SistemaTransporte::SistemaTransporte() : interfazGrafica(nullptr) {}
SistemaTransporte& SistemaTransporte::getInstance() { //Definición del metodo que accede al constructor privado y retorna la única instancia de SistemaTransporte
    static SistemaTransporte instancia;
    return instancia;
}

SistemaTransporte::~SistemaTransporte() {
    // Destruir buses, al llamar estos destructores, automaticamente también se destruyen los conductores
    for (const auto* bus : buses) {
            delete bus;
    }
    buses.clear();

    // Destruir rutas, esto elimina los horarios para rutas de barrio porque así está específicado
    for (const auto* ruta: rutas) {
            delete ruta; //No es necesario el <dynamic cast> debido a que el vector de rutas según el ajuste ahora solo contiene rutas de barrio
    }
    rutas.clear();

    delete interfazGrafica; //Comentado debido a que aún no existe una interfaz la cual destruir, temporalmente
    interfazGrafica = nullptr;
}

const std::vector<Ruta*>& SistemaTransporte::getRutas() const { //Getter del vector de rutas
    return rutas;
}

const std::vector<Bus*>& SistemaTransporte::getBuses() const { //Getter del vector de buses
    return buses;
}

Bus* SistemaTransporte::consultarBusPorPlaca(const std::string& placa) const { //Este metodo se encarga de buscar la placa ingresada dentro del vector de buses
//De SistemaTransporte, lo hace por medio de un for, si la encuentra, retorna el bus, si no, retorna nullptr
    for (auto* bus : buses) {
        if (bus->getPlaca() == placa) {
            return bus;
        }
    }
    return nullptr;
}

Ruta* SistemaTransporte::consultarRutaPorNombre(const std::string& nombre) const { //Mismo que el metodo anterior pero con el nombre de la ruta
    if (rutaCentro != nullptr && rutaCentro->getName() == nombre) { //Primero verifica que el puntero de RutaCentro no esté vacío y que su nombre coincida con el nombre buscado
        return rutaCentro;
    }
    for (auto* ruta : rutas) {
        if (ruta->getName() == nombre) {
            return ruta;
        }
    }
    return nullptr;
}

RutaCentro * SistemaTransporte::ruta_centro() const {
    return rutaCentro;
}

//Inicialización del metodo complejo para crear nuevas rutas, usa polimorfimo y memoria dinámica para decidir
//en tiempo de ejecución qué estructura de creación de instancias seguir según las instrucciones establecidas anteriormente
//Además de parsear o mapear los campos de los json a los atributos correspondientes de las rutas, los horarios y los paraderos
void SistemaTransporte::procesarYCrearRutas(const nlohmann::json& jRutas) {
    for (const auto& jRuta : jRutas) { //Ciclo for que recorre los objetos de rutas dentro del json ya procesado
        const std::string tipo = jRuta["tipo"];
        const std::string nombre = jRuta["nombre"];
        const bool activa = jRuta["activa"];
        const std::string descripcion = jRuta["descripcion"];

        Ruta* ruta = nullptr; //Asignación del puntero vacío de ruta porque aún no se conoce si es barrio o centro

        if (tipo == "centro") { //Si la ruta que se está procesando resulta ser de centro, se llama a la unica instancia de RutaCentro
            rutaCentro = &RutaCentro::getInstance();
            for (const auto& jParadero : jRuta["paraderos"]) { //Este metodo parsea los paraderos directamente porque
                //si es RutaCentro, el continue de la parte inferior hará que se salte el parseo de sus paraderos y el vector quedaría vacío
                const std::string nomParadero = jParadero.get<std::string>();
                const auto p = new Paradero(nomParadero, "");
                rutaCentro->agregarParadero(p);
            }
            continue;
        }

        if (tipo == "barrio") { //En cambio, si resulta ser de barrio, se parsea el campo de sector del json y se crea el objeto
            //de RutaBarrio en el heap, usando su constructor y parseando todos los artibutos antes sincronizados
            const std::string sector = jRuta["sector"];
            ruta = new RutaBarrio(nombre, activa, descripcion, sector);
        }

        if (ruta == nullptr) continue; //Si el puntero de ruta no apunta a un objeto, se salta la iteración actual

        for (const auto& jParadero : jRuta["paraderos"]) { //Este metodo lee los paraderos que tiene cada ruta y los crea como objetos únicos
            const std::string nomParadero = jParadero.get<std::string>(); //Esta línea se encarga de usar el metodo de jParadero para
            //traducir el texto plano del json y decirle al compilador que lo puede tratar como un string
            const auto p = new Paradero(nomParadero, "");
            ruta->agregarParadero(p); //Usa el metodo propio de ruta para asignarle este paradero a la última posición de su vector de paraderos
        }

        if (tipo == "barrio") {
            const auto rutaBarrio = dynamic_cast<RutaBarrio*>(ruta); //Esta línea hace la debida verificación de que el puntero de tipo
            //Ruta apunte a un objeto de tipo RutaBarrio, para que pueda acceder a los horarios propios de RutaBarrio y poder parsearlos según el json
            for (const auto& jHorario : jRuta["horarios"]) { //For que parsea los atributos de los horarios propios de rutaBarrio
                const std::string hora   = jHorario["hora"];
                const std::string minuto = jHorario["minuto"];
                const std::string estado = jHorario["estado"];
                const auto h = new Horario(hora, minuto, estado); //Se crea el apuntador que apunta a un objeto de tipo horario usando su contructor
                rutaBarrio->agregarHorario(h); //Se llama al metodo propio de ruta para agregar el horario
            }
        }

        rutas.push_back(ruta); //Se agrega la ruta creada a la última posición del vector de rutas
    }
}

void SistemaTransporte::procesarYCrearBuses(const nlohmann::json &jBuses) {
        for (const auto& jBus : jBuses) { //for que recorre el json de buses, ya parseado por el metodo cargarDatosDesdeArchivo
        const std::string placa = jBus["placa"];
        const int capacidad = jBus["capacidad"];
        const std::string nombreRuta = jBus["ruta"];
        const bool disponible = jBus["disponible"];
        const std::string observacion = jBus["observacion"];

        Ruta* ruta = nullptr; //Asignación del puntero vacío de ruta en caso de que el campo de ruta en el bus no exista
        if (!nombreRuta.empty()) { //Si en el json de bus el campo de nombre de Rutas no esté vacío, se hace lo siguiente, si está vacío, se omite este if
            ruta = consultarRutaPorNombre(nombreRuta);
        }
        auto bus = new Bus(placa, capacidad, ruta, disponible, observacion); //Se crea el puntero de bus, el tipo de dato es automatico porque el mismo compilador
        //intuye que como se llama el constructor de bus, lo que se devuelve es un puntero de tipo bus

        if (!jBus["conductor"].is_null()) { //Se revisa el campo del conductor en el json de bus, si este campo no tiene un null
        // el if se ejecuta, si sí tiene un null, el if se salta y no se parsean los datos del conductor a ninguna variable
            const std::string id = jBus["conductor"]["id"];
            const std::string nombre = jBus["conductor"]["nombre"];
            const std::string licencia = jBus["conductor"]["licencia"];
            const std::string telefono = jBus["conductor"]["telefono"];

            auto* conductor = new Conductor(id, nombre, licencia, telefono);
            bus->asignarConductor(conductor); //Se llama al metodo asignarConductor de la clase bus, a la vez que se hace esto, de manera implicita
            //se llama al metodo asignarConductor de la clase Conductor, así queda bien configurada la relación bidireccional que comparten estos dos
        }
        buses.push_back(bus); //Se agrega el objeto de bus antes creado con todas sus implicaciones a la parte final del vector de buses
        //Que la clase SistemaTransporte posee nativamente
    }
}

void SistemaTransporte::cargarDatosDesdeArchivo() {
    const std::string rutaArchivoRutas = construirRutaDatos("rutas_paraderos_horarios.json");
    const std::string rutaArchivoBuses = construirRutaDatos("buses_conductores.json");

    std::ifstream archivoRutas(rutaArchivoRutas); //Se llama a la libreria ifstream, esta se encarga de crear una variable local en donde se guardará el archivo abierto
    //Dentro del parentesis se pone el nombre del archivo que se va a abrir
    std::ifstream archivoBuses(rutaArchivoBuses);

    if (!archivoRutas.is_open()) { //Configuración de la incidencia usando la clase IncidenciaExcepcion, antes creada
        throw IncidenciaExcepcion("ARCHIVO NO ENCONTRADO", "No se ha podido abrir el archivo " + ARCHIVO_RUTAS);
    }

    if (!archivoBuses.is_open()) {
        throw IncidenciaExcepcion("ARCHIVO NO ENCONTRADO", "No se ha podido abrir el archivo " + ARCHIVO_BUSES);
    }

    nlohmann::json jRutas = nlohmann::json::parse(archivoRutas); //Se crea un archivo json EN LA RAM, parseando los archivos ya abiertos
    nlohmann::json jBuses = nlohmann::json::parse(archivoBuses);

    procesarYCrearRutas(jRutas); //Se llaman los métodos correspondientes y se les da los archivos .json antes creados EN RAM,
    //para que sean procesados y los objetos sean creados por medio de esos métodos
    procesarYCrearBuses(jBuses);
}

void SistemaTransporte::iniciarSistema() {
    cargarDatosDesdeArchivo();
    // Crear e iniciar la interfaz gráfica de consola
    interfazGrafica = new Interfaz(*this);
    try {
        interfazGrafica->mostrarBienvenida();
        interfazGrafica->mostrarPantalla2_TipoRuta();
    } catch (const std::exception& ex) {
        // Registrar en log en caso de excepcion
        try { Interfaz::registrarLogConsulta(std::string("EXCEPCION: ") + ex.what()); } catch(...){}
    }
    delete interfazGrafica;
    interfazGrafica = nullptr;
 }
