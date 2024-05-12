#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include "StorageManager.h"

void StorageManager::printTable(const std::string& filename) {

    std::ifstream a("C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt");
    std::string line;
    while (std::getline(a, line)) {
        if (line.find(filename) != std::string::npos) {
            std::stringstream ss(line);
            std::string palabras;
            std::getline(ss, palabras, '#');
            while (std::getline(ss, palabras, '#')) {
                std::cout << palabras << '\t';
            }  
        }   
    }
    std::cout << std::endl;
    a.close();

    std::string aux = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + filename + ".txt";
    std::ifstream file(aux);
    std::string lin;
    while (std::getline(file, lin)) {
        std::stringstream ss(lin);
        std::string palabras;
        while (std::getline(ss, palabras, '#')) {
            std::cout << palabras << '\t';
        }  
        std::cout << std::endl;
    }
    file.close();
}
void StorageManager::printTable(const std::string& filename, const std::string& columna, const std::string& condicion,int valor) {
    std::ifstream a("C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt");
    std::string line;
    int count = 0;
    int flag = 0;
    while (std::getline(a, line)) {
        if (line.find(filename) != std::string::npos) {
            std::stringstream ss(line);
            std::string palabras;
            std::getline(ss, palabras, '#');
            while (std::getline(ss, palabras, '#')) {
                if (palabras == columna){
                    flag = 1;
                    count++;
                }
                if (flag == 0)    
                    count++;
                std::cout << palabras << '\t';
            }  
        }
    }  
    std::cout << std::endl;
    
    a.close();

    std::string aux = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + filename + ".txt";
    std::ifstream file(aux);
    std::string lin;
    while (std::getline(file, lin)) {
        std::stringstream ss(lin);
        std::string palabras;

        for(int i = 0; i < count; i++) 
            std::getline(ss, palabras, '#');

        if( condicion =="<=" && std::atoi(palabras.c_str()) <= valor){
            std::stringstream ss(lin);
            std::string pal;
            while (std::getline(ss, pal, '#')) {
                std::cout << pal << '\t';
            }  
            std::cout << std::endl;
        }
        else if(condicion =="=" && std::atoi(palabras.c_str()) == valor ){
            std::stringstream ss(lin);
            std::string pal;
            while (std::getline(ss, pal, '#')) {
                std::cout << pal << '\t';
            } 
            std::cout << std::endl;
        }
        else if(condicion ==">=" && std::atoi(palabras.c_str()) >= valor){
            std::stringstream ss(lin);
            std::string pal;
            while (std::getline(ss, pal, '#')) {
                std::cout << pal << '\t';
            } 
            std::cout << std::endl;
        }  
        
    }
    file.close();
}

void StorageManager::parser(std::string &texto){
    std::string a = "SELECT * FROM Titanic WHERE PassengerId >= 40";
    std::regex patron("SELECT \\* FROM (\\w+)(?: WHERE (\\w+) ([<=>]+) (\\d+))?");
    // Busca el patrón en el texto
    std::smatch resultado;

    if (std::regex_search(texto, resultado, patron)) {

        std::string tabla = resultado[1];
        std::cout << "Se encontro la sintaxis 'SELECT * FROM <TABLA> WHERE <CONDICION>'." << std::endl;
        std::cout << "Tabla: " << tabla << std::endl;
        // Verifica si se encontraron condiciones después del WHERE
        
        std::string columna = resultado[2];
        std::string operador = resultado[3];
        std::string valor = resultado[4];
        std::cout << "Columna: " << columna << std::endl;
        std::cout << "Operador: " << operador << std::endl;
        std::cout << "Valor: " << valor << std::endl;
        if (!tabla.empty() && columna.empty())
            printTable(tabla);
        else
            printTable(tabla,columna,operador,std::atoi(valor.c_str()));
    } else {
        std::cout << "No se encontro la sintaxis 'SELECT * FROM <TABLA> WHERE <CONDICION>' en el texto." << std::endl;
    }
}
    
    
int StorageManager::VerificarDato(std::string dato) {
    int letra, numero, simbolo, fraccion;
    letra = numero = simbolo = fraccion = 0;
    if (dato[0] == '"' || dato[0] == ' '){
        dato = dato.substr(1);
    }
    if (dato[dato.length()-1] =='"')
        dato = dato.substr(0, dato.length() - 1);
    

    for(int i = 0; i < dato.length(); i++){

        if ( 65 <= dato[i] && dato[i] <= 90 || 97 <= dato[i] && dato[i] <= 122 || dato[i] == 32)
            letra++;
        else if (48 <= dato[i] && dato[i] <= 57)
            numero++;
        else if ( dato[i] == '.')
            fraccion++;
        else
            simbolo;
    }
    if ((65 <= dato[0] && dato[0] <= 90 || 97 <= dato[0] && dato[0] <= 122) && letra >= 1 && simbolo ==0) // el dato es un string
        return 0;
    else if ((65 <= dato[0] && dato[0] <= 90 || 65 <= dato[0] && dato[0] <= 90) && dato.length() == 1) // el dato es un char
        return 1;
    else if (numero >= 1 && letra == 0 && simbolo == 0 && fraccion == 0) // el dato es un entero
        return 2;
    else if ((48 <= dato[0] && dato[0] <= 57) && fraccion == 1 && letra == 0 && simbolo == 0) // el dato es un float
        return 3;
    else if (dato.length() == 1 && (dato[0] == '1' || dato[0] == '0')) // el dato es un bool 
        return 4;
    else
        return 5; // el dato no esta definido 
}

void StorageManager::LeerTabla(std::string tabla) {
    std::string nombre = tabla.substr(0, tabla.length() - 4);
    tabla = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + tabla;
    std::string direccionEsquema = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt";
    std::string direccionArchivo = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + nombre + ".txt";

    std::ofstream esquema(direccionEsquema, std::ios_base::app | std::ios_base::ate);
    if (!esquema.is_open()) {
        std::cerr << "Error al abrir el esquema." << std::endl;
        return;
    }
    esquema.seekp(0, std::ios::end);
    if (esquema.tellp() != 0) { 
        esquema << std::endl;
    }
    esquema << nombre;
 

    // Abre el archivo de entrada
    std::ifstream archivo(tabla);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    // Abre el archivo de salida
    std::ofstream Endd(direccionArchivo);
    if (!Endd.is_open()) {
        std::cerr << "Error al abrir el archivo Endd." << std::endl;
        archivo.close();
        return;
    }
        std::string linea;
        std::getline(archivo, linea);
        std::stringstream ss(linea);
        while (std::getline(ss, linea, ',')) {
            esquema << "#" << linea;        
        }
        esquema.close(); 

        while (std::getline(archivo, linea)) {
            std::stringstream ss(linea);
            std::string palabras;

            while (std::getline(ss, palabras, ',')) {
                if (palabras == "" || VerificarDato(palabras) == 5)
                    Endd << "NULL";
                else
                    Endd << palabras;

                if (ss.peek() != EOF && palabras[0] != '"') // Si no es el final de la línea, añade #
                    Endd << "#";
            }
            Endd << std::endl;
        }

    // Cierra los archivos después de leer/escribir
    Endd.close();
    archivo.close();
}


void StorageManager::ModificarDatos(std::string texto){ 
}

void StorageManager::AgregarTabla() {
    std::string aux;
    std::string nombre = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt";
     
    std::cout << "Nombre de la tabla: ";
    std::cin >> aux;

    std::ofstream archivo(nombre, std::ios_base::app | std::ios_base::ate);
    if (!archivo.is_open()){
        std::cerr << "Error al abrir el archivo." << std::endl;
    }
    archivo.seekp(0, std::ios::end);
    if (archivo.tellp() != 0) { // Si no está vacío, agregar nueva línea
        archivo << std::endl;
    }

    archivo << aux;

    char agregarMas;
        do {
        archivo << "#";
        std::cout << "Atributo: ";
        std::cin >> aux;
        archivo << aux + "#";
        std::cout << "Tipo de dato: ";
        std::cin >> aux;
        archivo << aux;
        std::cout << "¿Desea agregar otro atributo? (s/n): ";
        std::cin >> agregarMas;
    } while (agregarMas == 's' || agregarMas == 'S');

    

    archivo.close();
}
