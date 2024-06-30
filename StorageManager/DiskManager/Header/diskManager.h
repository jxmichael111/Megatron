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

namespace fs = std::filesystem;

#define RUTA_BASE "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\"

/*
    @author Andrea Cuela
*/
struct Nodo {
    int numeroBloque;
    int espacioLibre;
    std::unordered_map<int, std::tuple<int, char, int, bool>> sectores; // <idSector, <plato, superficie, pista, espacioSector>>

    Nodo* prev;
    Nodo* next;

    /*
        @author Andrea Cuela
    */
    Nodo(int numBloque, int espLibre)
        : numeroBloque(numBloque), espacioLibre(espLibre), prev(nullptr), next(nullptr) {
            std::cout << "Insertando bloque a nodo --> Numero de bloque: " << numeroBloque << ", Espacio libre: " << espacioLibre << std::endl;
        }

    /*
        @author Andrea Cuela
    */
    void agregarSector(int idSector, int plato, char superficie, int pista) {
        sectores[idSector] = std::make_tuple(plato, superficie, pista, false);
    }
};

/*
    @author Andrea Cuela
*/
struct CabeceraSector {
    std::optional<int> identificador;
    std::optional<int> espacioDisponible;
    std::optional<std::string> espaciosLibres;
    std::optional<int> numRegistros;

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
        if (numRegistros.has_value()) {
            cabecera += std::to_string(numRegistros.value()) + "#";
        }
        return cabecera;
    }
};

/*
    @author Andrea Cuela
*/
std::string removerPrimerElemento(const std::string&);


class DiskManager {
    private:        
        Disk disco; // Declaración de clase disco
        bool tipoLongitud; //TRUE si es longitud variable, FALSE si es fija
        int longitudRegistro; //Variable que guarda la longitud de registro en caso sea de LONGITUD FIJA

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
        void getBlockInformation(); //Imprime la cantidad de bloque y la longitud del mismo

        /*
        @author Andrea Cuela
        */
        void validarUbicacionActual(); //Hace el incremento de valores Act en caso lo necesite. Valida ubicaciones

        /*
        @author Andrea Cuela
        */
        void createStructureDisk(); //Crea la carpetas con la cantidad de sectores y bloques establecido

        /*
        @author Andrea Cuela
        */
        void setCurrentScheme(std::string nameScheme);

        /*
        @author Andrea Cuela
        */
        std::string getScheme();

        
        //void agregarBloquesEsquema(std::string); //? ni idea




        /*
        @author Andrea Cuela
        */
        void showBlockContent(int); //Imprime el contenido de un bloque

        /*
        @author Andrea Cuela
        */
        void showSectorContent(int, char, int, int); //Imprime el contenido de un sector

        /*
        @author Andrea Cuela
        */
        std::vector<std::string> readBlockToVector(int);


        // ================= HEAP FILE ==========
        
        /*
        @author Andrea Cuela
        */
        void insertBlocktoFreeHeapFile(int, int, const std::unordered_map<int, std::tuple<int, char, int, bool>>&);

        /*
        @author Andrea Cuela
        */
        void insertBlocktoFullHeapFile(int, int, const std::unordered_map<int, std::tuple<int, char, int, bool>>&);

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

        // !OJO

        /*
        @author Andrea Cuela
        */
        void saveHeapFile();

        /*
        @author Andrea Cuela
        */
        void saveInformationInFile(Nodo*, std::ofstream&);

        /*
        @author Andrea Cuela
        */
        void recoverInformationFromHeapFile();


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

        /*
        @author Andrea Cuela
        */
        void actualizarLineaLongitudFija(const std::string&, const std::string&, int);

        /*
        @author Andrea Cuela
        */
        void actualizarSector();


        // ================= LONGITUD VARIABLE ==============


        // ================= DE USO GENERAL - ARCHIVOS ======


        ~DiskManager();
};

#endif