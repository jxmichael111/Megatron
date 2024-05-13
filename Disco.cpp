#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "Disco.h"

std::string DiscoDuro::R_espacio(std::string letter, int num){
    int aux = num - letter.length();
    std::string a = letter;
    for(int i = 0; i < aux; i++)
        a = a + " ";
    return a;
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
    }
}
std::string DiscoDuro::getDireccion(){
    return ruta;
}


