#include <iostream>
#include <string>
  

using namespace std;

class DiscoDuro {
private:
   wstring ruta;
   int capacidad_total;
   int capacidad_utilizada;
   int espacio_libre;

public:
   DiscoDuro(int);
   void almacenarRegistro(int);
   void mostrarInformacion();
   bool Disponibilidad(const wstring&);
   void AddCarpeta(const wstring&);
};
