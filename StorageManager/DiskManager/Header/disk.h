#ifndef DISK_H
#define DISK_H

#include <iostream>
#include <string>
#include <sstream>

/* Manejo de Disco / espacio en disco*/
class Disk {
private:
    int nroPlatos;
    int nroSuperficies;
    int nroPistas;
    int nroSectores;
    int bytesxSector;
    int sectoresxBloque;
    std::string rutaBase;

public:

    /*
        @author Andrea Cuela
    */
    Disk();

    /*
        @author Andrea Cuela
    */
    Disk(int, int, int, int, int);

    /*
        @author Andrea Cuela
    */
    void printTamañoTotal();
    //long int getTamañoSobrante(long int);

    // * Operaciones get sobre información del disco

    /*
        @author Andrea Cuela
    */
    int getCantidadPlatos();

    /*
        @author Andrea Cuela
    */
    int getCantidadPistas();

    /*
        @author Andrea Cuela
    */
    int getCantidadSectores();

    /*
        @author Andrea Cuela
    */
    int getBytesxSector();

    /*
        @author Andrea Cuela
    */
    int getCantidadBloques();

    /*
        @author Andrea Cuela
    */
    int getCantidadSectoresxBloque();

    /*
        @author Andrea Cuela
    */
    int getTamañoBloque();

    /*
        @author Andrea Cuela
    */
    int getTotalSectores();

};

/*
    @author Andrea Cuela
*/
struct Registro {
    bool partition; //Indica si pertenece a uno o más registros
    int space; //Indica si el registro apun cuenta con espacio
};

#endif
