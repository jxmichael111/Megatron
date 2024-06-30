#include "disk.h"

#include <iostream>
#include <fstream>
#include <string>

Disk::Disk(int platos, int pistas, int sectores, int tamañoSector, int tamañoBloque) {
    this->nroPlatos = platos;
    this->nroSuperficies = 2;
    this->nroPistas = pistas;
    this->nroSectores = sectores;
    this->bytesxSector = tamañoSector;
    this->sectoresxBloque = tamañoBloque;
    this->rutaBase = std::string(RUTA_BASE); 
}

void Disk::printTamañoTotal() {
    std::cout << "Nro. Platos: " << this->nroPlatos << std::endl;
    std::cout << "Nro. Superficies: " << this->nroSuperficies << std::endl;
    std::cout << "Nro. Pistas: " << this->nroPistas << std::endl;
    std::cout << "Nro. Sectores: " << this->nroSectores << std::endl;
    std::cout << "Bytes x Sector: " << this->bytesxSector << std::endl;
    std::cout << "Sectores x Bloque: " << this->sectoresxBloque << std::endl;

    long long int total = static_cast<long long int>(this->nroPlatos) *
                            this->nroSuperficies *
                            this->nroPistas *
                            this->nroSectores *
                            this->bytesxSector;

    std::cout << "Tamanio Total: " << total << std::endl;
}

/*long int Disco::getTamañoSobrante(long int espacioOcupado) const {
    return getTamañoTotal() - espacioOcupado;
}*/

int Disk::getCantidadPlatos() {
    return this->nroPlatos;
}

int Disk::getCantidadPistas() {
    return this->nroPistas;
}

int Disk::getCantidadSectores() {
    return this->nroSectores;
}

int Disk::getBytesxSector() {
    return this->bytesxSector;
}

int Disk::getCantidadBloques() {
    int cantidadBloques = getTotalSectores() / sectoresxBloque;
    return cantidadBloques;
}

int Disk::getCantidadSectoresxBloque() {
    return this->sectoresxBloque;
}

int Disk::getTamañoBloque() {
    int tamaño = this->bytesxSector * this->sectoresxBloque;
    return tamaño;
}

int Disk::getTotalSectores() {
    int totalSectores = this->nroPlatos * this->nroPistas * this->nroSectores * 2;
    return totalSectores;
}