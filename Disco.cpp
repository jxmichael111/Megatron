#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include "Disco.h"

std::string DiscoDuro::R_espacio(std::string letter, int num){
    int aux = num - letter.length();
    std::string a = letter;
    for(int i = 0; i < aux; i++)
        a = a + " ";
    return a;
}

void DiscoDuro::crearCarpeta(std::string nombreCarpeta) {
    try {
        if (std::filesystem::create_directory(nombreCarpeta)) {
            std::cout << "Carpeta creada: " << nombreCarpeta << std::endl;
        } else {
            std::cout << "No se pudo crear la carpeta (ya existe o hubo otro error)." << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error al crear la carpeta: " << e.what() << std::endl;
    }
}
void DiscoDuro::crearArchivo(std::string nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (archivo) {
        std::cout << "Archivo creado: " << nombreArchivo << std::endl;
    } else {
        std::cerr << "No se pudo crear el archivo: " << nombreArchivo << std::endl;
    }
    archivo.close();

}
void DiscoDuro::estructura() { 
    crearCarpeta(ruta);

    for (int p = 0; p < platos; ++p) {
        std::string carpetaPlato = ruta + "/plato_" + std::to_string(p);
        crearCarpeta(carpetaPlato);
        
        for (int s = 0; s < 2; ++s) {
            std::string carpetaSuperficie = carpetaPlato + "/superficie_" + std::to_string(s);
            crearCarpeta(carpetaSuperficie);
            
            for (int pi = 0; pi < pistas; ++pi) {
                std::string carpetaPista = carpetaSuperficie + "/pista_" + std::to_string(pi);
                crearCarpeta(carpetaPista);
                
                for (int se = 0; se < sectores; ++se) {
                    std::string archivoSector = carpetaPista + "/sector_" + std::to_string(se) + ".txt";
                    crearArchivo(archivoSector);
                }
            }
        }
    }
}



DiscoDuro::DiscoDuro(int plato_, int pista_, int sector_, int bloque_ ) {
   this->ruta = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\Disco";
   this->c_sector = 512;
   this->platos = plato_;
   this->pistas = pista_;
   this->sectores = sector_;
   this->bloque = bloque_;
   Iniciar();
}

bool DiscoDuro::FileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}
void DiscoDuro::Iniciar(){
    std::string aux = ruta + "\\Directorio.txt";
    if (!FileExists(aux)) {
        std::string line;
        std::ofstream file(aux);
        if (file.is_open()) {
            int superficies = platos * 2;
            long long int total = static_cast<long long int>(platos) * static_cast<long long int>(superficies) * static_cast<long long int>(pistas) * static_cast<long long int>(sectores) * static_cast<long long int>(c_sector);
            std::cout << total << " " <<platos<< " " << superficies<< " " << pistas<< " " << sectores<< " " << c_sector<< " " <<std::to_string(total).length() << std::endl;
            file << std::to_string(total) << "#" << R_espacio("0", std::to_string(total).length()) << "#" << c_sector << "$" << platos << "#" << superficies << "#" << pistas << "#" << sectores << "#" << bloque;
            file.close();
        }   
        estructura();  
    }
}
std::string DiscoDuro::getDireccion(){
    return ruta;
}


