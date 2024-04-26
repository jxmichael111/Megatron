#include <iostream>
#include <fstream>
#include <string>
#include "StorageManager.h"


void StorageManager::VerificarDato(std::string dato) {
    if ( )
}

void StorageManager::LeerTabla(std::string tabla) {
    std::string nombre = tabla.substr(0, tabla.length()-4);

    
    std::ofstream esquema("esquema.txt");
    if (!esquema.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
    }
    esquema << nombre;
    
    esquema.close();

    std::ifstream archivo(tabla);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
    }


    std::string linea;
    while (std::getline(archivo, linea)) {
        std::cout << linea << std::endl;
    }

    // Cerrar el archivo después de leerlo
    archivo.close();

}

void StorageManager::ModificarDatos(std::string)
{
}

void StorageManager::AgregarTabla(){
    while ()
}
