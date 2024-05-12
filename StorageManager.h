#include <iostream>
#include <string>

class StorageManager {
private:
    int buffer[200];

    
public:
    int  VerificarDato(std::string);
    void LeerTabla(std::string);
    void ModificarDatos(std::string);
    void AgregarTabla();
    void printTable(const std::string&);
    void printTable(const std::string&, const std::string&, const std::string&, int);
    void parser(std::string&);
};

