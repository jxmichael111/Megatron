#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "StorageManager.h"

void StorageManager::printTable(const std::string& filename) {
    std::ifstream a("C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt");
    std::string line;
    while (std::getline(a, line)) {
        size_t pos = 0;
        std::string cell;
        if (line.find(filename) != std::string::npos) {
            while ((pos = line.find('#')) != std::string::npos) {
            cell = line.substr(0, pos);
            std::cout << cell << "\t" << "\t"; 
            line.erase(0, pos + 1);
        }
        }
    }
    std::cout << std::endl;
    a.close();

    std::string aux = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + filename + ".txt";
    std::ifstream file(aux);
    std::string lin;
    while (std::getline(file, lin)) {
        size_t pos = 0;
        std::string cell;
        while ((pos = lin.find('#')) != std::string::npos) {
            cell = lin.substr(0, pos);
            std::cout << cell << "\t" << "\t"; 
            lin.erase(0, pos + 1);
        }
        std::cout << lin << std::endl;
    }
    file.close();
}
void StorageManager::parser(std::string cadena){

}

void StorageManager::VerificarDato(std::string dato) {
}

void StorageManager::LeerTabla(std::string tabla) {
    std::string nombre = tabla.substr(65, tabla.length() - 8);
    std::string direccionEsquema = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt";
    std::string direccionArchivo = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + nombre + ".txt";

    std::ofstream esquema(direccionEsquema, std::ios_base::app | std::ios_base::ate);
    if (!esquema.is_open()) {
        std::cerr << "Error al abrir el esquema." << std::endl;
        return;
    }
    esquema.seekp(0, std::ios::end);
    if (esquema.tellp() != 0) { 
        esquema << std::endl;
    }
    esquema << nombre;
 

    // Abre el archivo de entrada
    std::ifstream archivo(tabla);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    // Abre el archivo de salida
    std::ofstream Endd(direccionArchivo);
    if (!Endd.is_open()) {
        std::cerr << "Error al abrir el archivo Endd." << std::endl;
        archivo.close();
        return;
    }

    std::string linea;
    std::getline(archivo, linea);
    std::stringstream ss(linea);
    while (std::getline(ss, linea, ',')) {
         esquema << "#" << linea;        
    }
    esquema.close(); 

    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string palabras;

        while (std::getline(ss, palabras, ',')) {
            if (palabras == "")
                Endd << "NULL";
            else
                Endd << palabras;
            Endd << "#";
        }
        Endd << std::endl;
    }

    // Cierra los archivos después de leer/escribir
    Endd.close();
    archivo.close();
}


void StorageManager::ModificarDatos(std::string texto){ 
}

void StorageManager::AgregarTabla() {
    std::string aux;
    std::string nombre = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt";
     
    std::cout << "Nombre de la tabla: ";
    std::cin >> aux;

    std::ofstream archivo(nombre, std::ios_base::app | std::ios_base::ate);
    if (!archivo.is_open()){
        std::cerr << "Error al abrir el archivo." << std::endl;
    }
    archivo.seekp(0, std::ios::end);
    if (archivo.tellp() != 0) { // Si no está vacío, agregar nueva línea
        archivo << std::endl;
    }

    archivo << aux;

    char agregarMas;
        do {
        archivo << "#";
        std::cout << "Atributo: ";
        std::cin >> aux;
        archivo << aux + "#";
        std::cout << "Tipo de dato: ";
        std::cin >> aux;
        archivo << aux;
        std::cout << "¿Desea agregar otro atributo? (s/n): ";
        std::cin >> agregarMas;
    } while (agregarMas == 's' || agregarMas == 'S');

    

    archivo.close();
}
