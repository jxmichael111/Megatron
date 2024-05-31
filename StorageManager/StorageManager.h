#include <iostream>
#include <string>
#include <vector>

class StorageManager {
private:
    int buffer[200];

    
public:
    int  VerificarDato(std::string);
    void AddEsquema(std::string);
    void LeerTabla(std::string,int);
    void ModificarDatos(std::string);
    void AgregarTabla();
    void printTable(const std::string&);
    void printTable(const std::string&, const std::string&, const std::string&, int); /*modificar para que reconozca mas columnas*/
    void parser(std::string&);  /*incluir analisador lexico*/
    void DatoVariable(std::string,int);
    void DatoFijo(std::string,int);
    std::vector<int> ColumnaSize(std::string);
    std::vector<std::string> ColumnaTipo(std::string);
    std::string R_espacio(std::string, int);
};

