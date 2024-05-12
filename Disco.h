#include <iostream>
#include <fstream>
#include <string>
  

using namespace std;

class DiscoDuro {
private:
   string ruta;
   string platos; 
   string pistas;
   string sectores;
   string bloque;
public:
   DiscoDuro(string,string,string,string); // tamaño del disco en MB
   bool FileExists(const std::string&);
   void Iniciar();
   string getDireccion();
};
