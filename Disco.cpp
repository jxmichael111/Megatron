#include <iostream>
#include <string>
#include <windows.h>
#include "Disco.h"

using namespace std;

DiscoDuro::DiscoDuro(int capacidad_total) {
   this->ruta = L"C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\Disco";
   this->capacidad_total = capacidad_total;
   this->capacidad_utilizada = 0;
   this->espacio_libre = capacidad_total;
}
void DiscoDuro::almacenarRegistro(int registro) {
   int bytes_registro = registro;
   if (espacio_libre >= bytes_registro) {
       capacidad_utilizada += bytes_registro;
       espacio_libre -= bytes_registro;
       cout << "Registro almacenado correctamente. Espacio libre: " << espacio_libre << " bytes." << endl;
   } else {
       cout << "Error: No hay suficiente espacio en el disco duro para almacenar este registro." << endl;
   }
}
void DiscoDuro::mostrarInformacion() {
   cout << "Capacidad total del disco duro: " << capacidad_total << " bytes." << endl;
   cout << "Capacidad utilizada: " << capacidad_utilizada << " bytes." << endl;
   cout << "Espacio libre: " << espacio_libre << " bytes." << endl;
}   
bool DiscoDuro::Disponibilidad(const std::wstring& ruta) {
    // Intentar crear el directorio
    if (CreateDirectoryW(ruta.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        return true;
    } else {
        return false;
    }
}
void DiscoDuro::AddCarpeta(const wstring& nombre) {
   std::wstring aux = ruta + L"\\" + nombre;
   if (Disponibilidad(aux)) {
        std::wcout << L"Se ha creado la carpeta correctamente en: " << aux << std::endl;
    } else {
        std::wcerr << L"Error al crear la carpeta." << std::endl;
    }

}
