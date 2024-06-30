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
   int sectoresPorBloque = 5;
   int sectoresUsados = 0;
      
public:
   DiscoDuro(int,int,int,int); // platos,pistas,sectores,bloques
   bool FileExists(const std::string&);   
   void Iniciar();
   string getDireccion();
   string R_espacio(std::string, int); //reserva espacio
   void crearCarpeta(std::string);
   void crearArchivo(std::string);
   void estructura();

};
