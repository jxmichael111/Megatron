#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include "disk.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <tuple>
#include <list>
#include <optional>
#include <cstdio>
#include <vector>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>

#include "config.h"
#include "../../AccessMethods/Header/BPTree.h"

namespace fs = std::filesystem;

/*
    @author Andrea Cuela
*/
struct Nodo {
    int numeroBloque;
    int espacioLibre;
    std::vector<std::tuple<int, char, int, int, bool>> sectores; // <plato, superficie, pista, idSector, espacioSector>

    Nodo* prev;
    Nodo* next;

    Nodo(int numBloque, int espLibre)
        : numeroBloque(numBloque), espacioLibre(espLibre), prev(nullptr), next(nullptr) {
        std::cout << "Insertando bloque a nodo --> Numero de bloque: " << numeroBloque << ", Espacio libre: " << espLibre << std::endl;
    }

    void agregarSector(int plato, char superficie, int pista, int idSector, bool espacioSector) {
        sectores.push_back(std::make_tuple(plato, superficie, pista, idSector, espacioSector));
    }
};

/*
    @author Andrea Cuela
*/
struct CabeceraBloque {
    std::optional<int> identificador;
    std::optional<int> espacioDisponible;
    std::optional<std::string> espaciosLibres;
    //std::optional<int> numRegistros;

    /*
        @author Andrea Cuela
    */
    // Función para convertir la cabecera a una cadena
    std::string toString() const {
        std::string cabecera;
        if (identificador.has_value()) {
            cabecera += std::to_string(identificador.value()) + "#";
        }
        if (espacioDisponible.has_value()) {
            cabecera += std::to_string(espacioDisponible.value()) + "#";
        }
        if (espaciosLibres.has_value()) {
            cabecera += espaciosLibres.value() + "#";
        }
        return cabecera;
    }
};

struct CabeceraSector {
    std::optional<int> identificador;
    std::optional<int> espacioDisponible;
    std::optional<std::string> espaciosLibres;
    std::optional<std::string> ubicacion;

    /*
        @author Andrea Cuela
    */
    // Función para convertir la cabecera a una cadena
    std::string toString() const {
        std::string cabecera;
        if (identificador.has_value()) {
            cabecera += std::to_string(identificador.value()) + "#";
        }
        if (espacioDisponible.has_value()) {
            cabecera += std::to_string(espacioDisponible.value()) + "#";
        }
        if (espaciosLibres.has_value()) {
            cabecera += espaciosLibres.value() + "#";
        }
        if (ubicacion.has_value()) {
            cabecera += ubicacion.value() + "#";
        }
        return cabecera;
    }
};


class DiskManager {
    private:        
        Disk disco; // Declaración de clase disco

        bool tipoLongitud; //TRUE si es longitud variable, FALSE si es fija
        int longitudRegistro; //Variable que guarda la longitud de registro en caso sea de LONGITUD FIJA
        
        //Índices guía:
        int platoAct;
        char superfAct;
        int pistaAct;
        int sectorAct;
        int bloqueAct;

        //HeapFile -- FreeSpace
        Nodo* freeSpaceInicial; //Puntero hacia el nodo inicial de la linked list
        Nodo* freeSpaceFinal; //Puntero hacia el nodo final de la linked list

        //HeapFile -- FullSpace
        Nodo* fullSpaceInicial; //Puntero hacia el nodo inicial de la linked list
        Nodo* fullSpaceFinal; //Puntero hacia el nodo final de la linked list


        int* tipoCampo; //Puntero hacia un arreglo de tipos de campo del esquema seleccionado
        int numCampos; //Variable que guarda la cantidad de campos del esquema seleccionado

    public:
    
        /*
        @author Andrea Cuela
        */
        DiskManager();

        /*
        @author Andrea Cuela
        */
        DiskManager(bool longitud, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque);

        // ================ GENERAL =========================

        /*
        @author Andrea Cuela
        */
        void showInformation(); //Imprime la información del disco

        /*
        @author Andrea Cuela
        */
        void getBlockInformation(); //Imprime la cantidad de bloques y la longitud del mismo

        /*
        @author Andrea Cuela
        */
        void validarUbicacionActual(); //Hace el incremento de valores Act en caso lo necesite. Valida ubicaciones

        /*
        @author Andrea Cuela
        */
        void createStructureDisk(); //Crea la carpetas con la cantidad de sectores y bloques establecido
        
        //void agregarBloquesEsquema(std::string); //? ni idea

        /*
        @author Andrea Cuela
        */
        void actualizarSector();

        /*
        @author Andrea Cuela
        */
        void actualizarBloque();

        /*
        @author Andrea Cuela
        */
        void showBlockContent(int); //Imprime el contenido de un bloque

        /*
        @author Andrea Cuela
        */
        void showSectorContent(int, char, int, int); //Imprime el contenido de un sector

        // ================ CONEXIÓN CON BUFFER ============

        void insertar(std::string linea);

        void actualizar(int numBloque, std::string linea, int numRegistro);

        void eliminar(int numBloque, int numRegistro);

        std::string leer(int numBloque, int numRegistro);



        /*
        @author Andrea Cuela
        */
        std::vector<std::string> readBlockToVector(int);

        /*
        @author Andrea Cuela
        */
        std::tuple<int, int> buscarID(const std::string&);


        // ================= HEAP FILE =====================
        
        /*
        @author Andrea Cuela
        */
        void insertBlocktoFreeHeapFile(int, int, const std::vector<std::tuple<int, char, int, int, bool>>&);

        /*
        @author Andrea Cuela
        */
        void insertBlocktoFullHeapFile(int, int, const std::vector<std::tuple<int, char, int, int, bool>>&);

        /*
        @author Andrea Cuela
        */
        void showFullHeapFile();

        /*
        @author Andrea Cuela
        */
        void printBlockInformation(Nodo*);

        /*
        @author Andrea Cuela
        */
        void printBlockHeapFile(int);

        /*
        @author Andrea Cuela
        */
        Nodo* searchFreeSpace(int);

        /*
        @author Andrea Cuela
        */
        Nodo* searchFullSpace(int);

        /*
        @author Andrea Cuela
        */
        Nodo* searchBlockHeapFile(int);

        /*
        @author Andrea Cuela
        */
        void decreaseSpaceofBlock(int);

        /*
        @author Andrea Cuela
        */
        void increaseSpaceofBlock(int);

        /*
        @author Andrea Cuela
        */
        void deleteBlockHeapFile(Nodo*&, Nodo*&, Nodo*);

        /*
        @author Andrea Cuela
        */
        void moveBlockFreeToFull(Nodo*);

        /*
        @author Andrea Cuela
        */
        void moveBlockFullToFree(Nodo*);

        /*
        @author Andrea Cuela
        */
        void vaciarHeapFile(Nodo*&, Nodo*&); //Sobrecarga de funciones

        /*
        @author Andrea Cuela
        */
        void vaciarHeapFile();

        /*
        @author Andrea Cuela
        */
        void guardarHeapFile();

        void recuperarHeapFile();


        // ================= LONGITUD FIJA ==================

        /*
        @author Andrea Cuela
        */
        void setSizeScheme(const std::string&);

        /*
        @author Andrea Cuela
        */
        void setLongitudRegistro(int); //Indica la longitud de registro en caso se considere un disco de LONGITUD FIJA

        /*
        @author Andrea Cuela
        */
        void useLongitudFija(const std::string);

        /*
        @author Andrea Cuela
        */
        void sectorFillLongitudFija(const std::string&, int, Nodo*&);

        void actualizarLineaLongitudFija(int,const std::string&, int);

        /*
        @author Andrea Cuela
        */
        void actualizarLineaLongitudFija(const std::string&, const std::string&, int);

        void eliminarLineaLongitudFija(int numBloque, int numRegistro);

        // ================= LONGITUD VARIABLE ==============


        // ================= DE USO GENERAL - ARCHIVOS ======


        ~DiskManager();
};

#endif