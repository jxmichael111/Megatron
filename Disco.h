#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class DiscoDuro {
private:
   int c_sector;
   string ruta;
      int platos; 
      int pistas;
      int sectores;
      int bloque;
public:
   DiscoDuro(int,int,int,int); // platos,pistas,sectores,bloques
   bool FileExists(const std::string&);   
   void Iniciar();
   string getDireccion();
   string R_espacio(std::string, int); //reserva espacio
};
