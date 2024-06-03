#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include "StorageManager.h"

void StorageManager::printTable(const std::string &filename)
{

    std::ifstream a("C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt");
    std::string line;
    while (std::getline(a, line))
    {
        if (line.find(filename) != std::string::npos)
        {
            std::stringstream ss(line);
            std::string palabras;
            std::getline(ss, palabras, '$');
            while (std::getline(ss, palabras, '$'))
            {
                std::stringstream ss(palabras);
                std::string p;
                std::getline(ss, p, '#');
                std::cout << p << '\t';
            }
        }
    }
    std::cout << std::endl;
    a.close();

    std::string aux = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + filename + ".txt";
    std::ifstream file(aux);
    std::string lin;
    while (std::getline(file, lin))
    {
        std::stringstream ss(lin);
        std::string palabras;
        while (std::getline(ss, palabras, '#'))
        {
            std::cout << palabras << '\t';
        }
        std::cout << std::endl;
    }
    file.close();
}
void StorageManager::printTable(const std::string &filename, const std::string &columna, const std::string &condicion, int valor)
{
    std::ifstream a("C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt");
    std::string line;
    int count = 0;
    int flag = 0;
    while (std::getline(a, line))
    {
        if (line.find(filename) != std::string::npos)
        {
            std::stringstream ss(line);
            std::string palabras;
            std::getline(ss, palabras, '$');
            while (std::getline(ss, palabras, '$'))
            {
                std::stringstream ss(palabras);
                std::string p;
                std::getline(ss, p, '#');
                if (p == columna)
                {
                    flag = 1;
                    count++;
                }
                if (flag == 0)
                    count++;
                std::cout << p << '\t';
            }
        }
    }
    std::cout << std::endl;

    a.close();

    std::string aux = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + filename + ".txt";
    std::ifstream file(aux);
    std::string lin;
    while (std::getline(file, lin))
    {
        std::stringstream ss(lin);
        std::string palabras;

        for (int i = 0; i < count; i++)
            std::getline(ss, palabras, '#');

        if (condicion == "<=" && std::atoi(palabras.c_str()) <= valor)
        {
            std::stringstream ss(lin);
            std::string pal;
            while (std::getline(ss, pal, '#'))
            {
                std::cout << pal << '\t';
            }
            std::cout << std::endl;
        }
        else if (condicion == "=" && std::atoi(palabras.c_str()) == valor)
        {
            std::stringstream ss(lin);
            std::string pal;
            while (std::getline(ss, pal, '#'))
            {
                std::cout << pal << '\t';
            }
            std::cout << std::endl;
        }
        else if (condicion == ">=" && std::atoi(palabras.c_str()) >= valor)
        {
            std::stringstream ss(lin);
            std::string pal;
            while (std::getline(ss, pal, '#'))
            {
                std::cout << pal << '\t';
            }
            std::cout << std::endl;
        }
    }
    file.close();
}

void StorageManager::parser(std::string &texto)
{
    std::string a = "SELECT * FROM Titanic WHERE PassengerId >= 40";
    std::regex patron("SELECT \\* FROM (\\w+)(?: WHERE (\\w+) ([<=>]+) (\\d+))?");
    // Busca el patrón en el texto
    std::smatch resultado;

    if (std::regex_search(texto, resultado, patron))
    {

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
            printTable(tabla, columna, operador, std::atoi(valor.c_str()));
    }
    else
    {
        std::cout << "No se encontro la sintaxis 'SELECT * FROM <TABLA> WHERE <CONDICION>' en el texto." << std::endl;
    }
}

std::string StorageManager::DatoVariable(std::string letter, int num)
{
    int aux = num - letter.length();
    std::string a = letter;
    return a;
}

std::string StorageManager::DatoFijo(std::string letter, int num){
    int aux = num - letter.length();
    std::string a = letter;
    for(int i = 0; i < aux; i++)
        a = a + " ";
    return a;
}

std::vector<int> StorageManager::ColumnaSize(std::string nombre)
{
    std::string direccionEsquema = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt";
    std::vector<int> temp;
    std::ifstream esquema(direccionEsquema);
    if (!esquema.is_open())
    {
        std::cerr << "Error al abrir el esquema." << std::endl;
        return temp;  // Devolver un vector vacío en caso de error
    }
    std::string linea;
    while (std::getline(esquema, linea))
    {
        if (linea.find(nombre) != std::string::npos)
        {
            std::stringstream ss(linea);
            std::string palabras;
            std::getline(ss, palabras, '$'); // Saltar el nombre
            while (std::getline(ss, palabras, '$'))
            {
                std::stringstream ss2(palabras);
                std::string p;
                std::getline(ss2, p, '#'); // Saltar el primer campo
                std::getline(ss2, p, '#'); // Saltar el segundo campo
                std::getline(ss2, p, '#'); // Obtener el tamaño
                temp.push_back(std::atoi(p.c_str()));
            }
        }
    }

    return temp;    
}
std::vector<std::string> StorageManager::ColumnaTipo(std::string nombre)
{
    std::string direccionEsquema = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt";
    std::vector<std::string> temp;
    std::ifstream esquema(direccionEsquema);
    if (!esquema.is_open())
    {
        std::cerr << "Error al abrir el esquema." << std::endl;
        return temp;  // Devolver un vector vacío en caso de error
    }
    std::string linea;
    while (std::getline(esquema, linea))
    {
        if (linea.find(nombre) != std::string::npos)
        {
            std::stringstream ss(linea);
            std::string palabras;
            std::getline(ss, palabras, '$'); // Saltar el nombre
            while (std::getline(ss, palabras, '$'))
            {
                std::stringstream ss2(palabras);
                std::string p;
                std::getline(ss2, p, '#'); // Saltar el primer campo
                std::getline(ss2, p, '#'); // Saltar el segundo campo
                temp.push_back(p);
            }
        }
    }

    return temp;    
}


int StorageManager::VerificarDato(std::string dato)
{
    int letra, numero, simbolo, fraccion;
    letra = numero = simbolo = fraccion = 0;

    for (int i = 0; i < dato.length(); i++)
    {

        if (65 <= dato[i] && dato[i] <= 90 || 97 <= dato[i] && dato[i] <= 122 || dato[i] == 32)
            letra++;
        else if (48 <= dato[i] && dato[i] <= 57)
            numero++;
        else if (dato[i] == '.')
            fraccion++;
        else
            simbolo;
    }
    
    if (dato[0] == '"' || dato[0] == ' ' ||((65 <= dato[0] && dato[0] <= 90 || 97 <= dato[0] && dato[0] <= 122) && letra > 1 && simbolo ==0)) // el dato es un string
        return 0;
    else if ((65 <= dato[0] && dato[0] <= 90 || 97 <= dato[0] && dato[0] <= 122) && dato.length() == 1) // el dato es un char
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

void StorageManager::AddEsquema(std::string tabla)
{
    std::string nombre = tabla.substr(0, tabla.length() - 4);
    tabla = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + tabla;
    std::string direccionEsquema = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt";

    std::ofstream esquema(direccionEsquema, std::ios_base::app | std::ios_base::ate);
    if (!esquema.is_open())
    {
        std::cerr << "Error al abrir el esquema." << std::endl;
        return;
    }
    esquema.seekp(0, std::ios::end);
    if (esquema.tellp() != 0)
    {
        esquema << std::endl;
    }
    esquema << nombre;

    
    // Abre el archivo de entrada
    std::ifstream archivo(tabla);
    if (!archivo.is_open())
    {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    std::getline(archivo, linea);
    std::stringstream ss(linea);
    int count = 1;
    while (std::getline(ss, linea, ','))
    {
        int array[7] = {0,0,0,0,0,0,0};
        std::ifstream arc(tabla);
        std::string aux;
        std::getline(arc, aux);
        
        while (std::getline(arc, aux))
        {   	
            std::stringstream sss(aux);
            std::string cadena;
            std::string cadena_anterior = cadena ;
            for (int i = 0; i < count; i++)
            {
                std::getline(sss, cadena, ',');
                if(cadena[0] == '"'){
                    std::string temp = cadena;
                    std::getline(sss, cadena, ',');
                    cadena = temp + cadena;
                } 
            }
            
            if (VerificarDato(cadena) == 0)
            {
                array[0]++;
                if (array[6] < cadena.length() ){
                    array[6] = cadena.length();
                    if(cadena[0] == '"')
                        array[0] -= 2;
                }
            }
            else if (VerificarDato(cadena) == 1)
            {
                array[1]++;
                if (array[6] < cadena.length())
                    array[6] = cadena.length();
            }
            else if (VerificarDato(cadena) == 2)
            {
                array[2]++;
                if (array[6] < cadena.length())
                    array[6] = cadena.length();
            }
            else if (VerificarDato(cadena) == 3)
            {
                array[3]++;
                if (array[6] < cadena.length())
                    array[6] = cadena.length()-1;
            }
            else if (VerificarDato(cadena) == 4)
            {
                array[4]++;
                if (array[6] < cadena.length())
                    array[6] = cadena.length();
            }
            else
            {
                array[5]++;
                if (array[6] < cadena.length())
                    array[6] = cadena.length();
            }
        }
        /*
        for (int i = 0 ; i < 7; i ++)
            std::cout << array[i] << " ";
        std::cout<<std::endl;
        //*/
        esquema << "$" << linea;
        int indiceMaximo = 0; // Inicializamos el índice del máximo como el primer elemento del array
        for (int i = 0; i < 6; ++i)
            {
                if (array[i] > array[indiceMaximo]) {
                indiceMaximo = i; // Actualizamos el índice del máximo si encontramos un elemento mayor
            }
        }
        if (indiceMaximo == 0)
            esquema << "#" << "STR" <<"#"<< array[6];
        else if (indiceMaximo == 1)
            esquema << "#" << "CHAR" <<"#"<< array[6];
        else if (indiceMaximo == 2)
            esquema << "#" << "INT" <<"#"<< array[6];
        else if (indiceMaximo == 3)
            esquema << "#" << "FLOAT" <<"#"<< array[6];
        else if (indiceMaximo == 4)
            esquema << "#" << "BOOL" <<"#"<< array[6];
        else
            esquema << "#" << "STR" <<"#"<< array[6];
        arc.close();
        count++;
    }
    esquema.close();
    archivo.close();
}

void StorageManager::LeerTabla(std::string tabla,int num)
{
    AddEsquema(tabla);
    std::string nombre = tabla.substr(0, tabla.length() - 4);
    std::vector<int> tam = ColumnaSize(nombre);
    std::vector<std::string> tipo = ColumnaTipo(nombre);

    tabla = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + tabla;
    std::string direccionArchivo = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\" + nombre + ".txt";

    // Abre el archivo de entrada
    std::ifstream archivo(tabla);
    if (!archivo.is_open())
    {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    // Abre el archivo de salida
    std::ofstream Endd(direccionArchivo);
    if (!Endd.is_open())
    {
        std::cerr << "Error al abrir el archivo Endd." << std::endl;
        archivo.close();
        return;
    }
    std::string linea;
    std::getline(archivo, linea);
    std::stringstream ss(linea);
    if(num == 0){
        while (std::getline(archivo, linea))
        {
            std::stringstream ss(linea);
            std::string palabras;
            int count = 0;
            while (std::getline(ss, palabras, ','))
            {
                if(palabras[0] == '"'){
                    std::string temp = palabras;
                    std::getline(ss, palabras, ',');
                    temp = temp + palabras;
                    palabras = temp;
                }
                if (palabras == "" || VerificarDato(palabras) == 5){
                    Endd << DatoFijo("", tam[count]);
                    count++;
                }
                else if (tipo[count] == "STR"){
                    //Endd << palabras;
                    Endd << DatoFijo(palabras, tam[count]);
                    count++;
                }
                else{
                    Endd << DatoFijo(palabras, tam[count]);
                    count++;
                }
                    

                if (ss.peek() != EOF) // Si no es el final de la línea, añade #
                    Endd << "#";
            }
            Endd << std::endl;
        }
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            if(!std::getline(archivo, linea))
                break;
            std::stringstream ss(linea);
            std::string palabras;
            int count = 0;
            while (std::getline(ss, palabras, ','))
            {
                if(palabras[0] == '"'){
                    std::string temp = palabras;
                    std::getline(ss, palabras, ',');
                    temp = temp + palabras;
                    palabras = temp;
                }
                if (palabras == "" || VerificarDato(palabras) == 5){
                    Endd << DatoFijo("", tam[count]);
                    count++;
                }
                else if (tipo[count] == "STR"){
                    
                    //Endd << palabras;
                    Endd << DatoFijo(palabras, tam[count]);
                    count++;
                }
                else{
                    Endd << DatoFijo(palabras, tam[count]);
                    count++;
                }

                if (ss.peek() != EOF) // Si no es el final de la línea, añade #
                    Endd << "#";
            }
            Endd << std::endl;
        }
        
    }
    
    // Cierra los archivos después de leer/escribir
    Endd.close();
    archivo.close();
}

void StorageManager::ModificarDatos(std::string texto)
{
}

void StorageManager::AgregarTabla()
{
    std::string aux;
    std::string nombre = "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\esquema.txt";

    std::cout << "Nombre de la tabla: ";
    std::cin >> aux;

    std::ofstream archivo(nombre, std::ios_base::app | std::ios_base::ate);
    if (!archivo.is_open())
    {
        std::cerr << "Error al abrir el archivo." << std::endl;
    }
    archivo.seekp(0, std::ios::end);
    if (archivo.tellp() != 0)
    { // Si no está vacío, agregar nueva línea
        archivo << std::endl;
    }

    archivo << aux;

    char agregarMas;
    do
    {
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
