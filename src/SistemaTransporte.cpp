//
// Created by Nicolle on 5/26/2026.
//

#include "../include/SistemaTransporte.h"
// #include "../include/Interfaz.h"
#include "../include/RutaBarrio.h"
#include "../include/RutaCentro.h"
#include "../include/Conductor.h"
#include "../include/Paradero.h"
#include "../include/Horario.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include "IncidenciaExepcion.h"

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
    for (auto* ruta: rutas) {
        if (dynamic_cast<RutaCentro*>(ruta) == nullptr) { //Pregunta si el puntero que se está evaluando es RutaCentro, si es así lo ignora y no hace doble eliminación para evitar crasheos
            delete ruta;
        }
    }
    rutas.clear();

    //delete interfazGrafica; //Comentado debido a que aun no existe una interfaz la cual destruir, temporalmente
    //interfazGrafica = nullptr;
}

const std::vector<Ruta*>& SistemaTransporte::getRutas() const { //Getter del vector de butas
    return rutas;
}

const std::vector<Bus*>& SistemaTransporte::getBuses() const { //Getter del vector de buses
    return buses;
}

Bus* SistemaTransporte::consultarBusPorPlaca(const std::string& placa) const { //Este metodo se encarga de buscar el la placa ingresada dentro del vector de buses
//De SistemaTransporte, lo hace por medio de un for, si la encuentra, retorna el bus, si no, retorna nullptr
    for (auto* bus : buses) {
        if (bus->placa1() == placa) {
            return bus;
        }
    }
    return nullptr;
}

Ruta* SistemaTransporte::consultarRutaPorNombre(const std::string& nombre) const { //Mismo que el metodo anterior pero con el nombre de la ruta
    for (auto* ruta : rutas) {
        if (ruta->nombre1() == nombre) {
            return ruta;
        }
    }
    return nullptr;
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
            ruta = &RutaCentro::getInstance();
        } else if (tipo == "barrio") { //En cambio, si resulta ser de barrio, se parsea el campo de sector del json y se crea el objeto
        //de RutaBarrio en el heap, usando su constructor y parseando todos los artibutos antes sincronizados
            const std::string sector = jRuta["sector"];
            ruta = new RutaBarrio(nombre, activa, descripcion, sector);
        }

        if (ruta == nullptr) continue;

        for (const auto& jParadero : jRuta["paraderos"]) { //Este metodo lee los paraderos que tiene cada ruta y los crea como objetos únicos
            const std::string nomParadero = jParadero.get<std::string>(); //Esta línea se encarga de usar el metodo de jParadero para
            //traducir el texto plano del json y decirle al compilador que lo puede tratar como un string
            const auto p = new Paradero(nomParadero, "");
            ruta->agregarParadero(p); //Usa el metodo propio de ruta para asignarle este paradero a la última posición de su vector de paraderos
        }

        if (tipo == "barrio") {
            const auto rutaBarrio = dynamic_cast<RutaBarrio*>(ruta); //Esta linea hace la debida verificación de que la el puntero de tipo
            //Ruta apunte a un objeto de tipo RutaBarrio, para que pueda acceder a los atributos propios de RutaBarrio y poder parsearlos según el json
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
            bus->asignarConductor(conductor); //Se  llama al metodo asignarConductor de la clase bus, a la vez que se hace esto, de manera implicita
            //se llama al metodo asignarConductor de la clase Conductor, así queda bien configurada la relación bidireccional que comparten estos dos
        }
        buses.push_back(bus); //Se agrega el objeto de bus antes creado con todas sus implicaciones a la parte final del vector de buses
        //Que la clase SistemaTransporte posee nativamente
    }
}

void SistemaTransporte::cargarDatosDesdeArchivo() {
    std::ifstream archivoRutas(ARCHIVO_RUTAS); //Se llama a la libreria ifstream, esta se encarga de crear una variable local en donde se guardará el archivo abierto
    //Dentro del parentesis se pone el nombre del archivo que se va a abrir
    std::ifstream archivoBuses(ARCHIVO_BUSES);

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
