#include <iostream>
#include <string>
  

using namespace std;

class DiscoDuro {
private:
   wstring ruta;
   int capacidad_total;
   int capacidad_utilizada;
   int espacio_libre;
   int platos;
   int superficies;
   int pistas;
   int bloques;
   int sectores;


public:
   DiscoDuro(int); // tamaño del disco en MB
   void almacenarRegistro(int);
   void mostrarInformacion();
   bool Disponibilidad(const wstring&);
   void AddCarpeta(const wstring&);
};
