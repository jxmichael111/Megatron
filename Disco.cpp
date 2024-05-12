#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "Disco.h"

std::string DiscoDuro::getDireccion(){
    return ruta;
}

DiscoDuro::DiscoDuro(std::string plato_, std::string pista_, std::string sector_, std::string bloque_ ) {
   this->ruta = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\Disco";
   this->platos = plato_;
   this->pistas = pista_;
   this->sectores = sector_;
   this->bloque = bloque_;
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
            int num = std::atoi(platos.c_str());
            num *= 2;
            std::string result = std::to_string(num);
            file << platos << " " << result << " " << pistas << " " << sectores << " " << bloque << "#0 0 0 0 0";
            file.close();
        }     
    }
}



