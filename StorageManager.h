#include <iostream>
#include <string>

class StorageManager {
private:
    int buffer[200];

    
public:
    void VerificarDato(std::string);
    void LeerTabla(std::string);
    void ModificarDatos(std::string);
    void AgregarTabla();
    void printTable(const std::string&);
    void parser(std::string);
};

