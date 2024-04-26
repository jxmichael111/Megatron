#include <iostream>
#include <string>
#include <conio.h>

int main() {
    int opcion = 0;
    char tecla;
    std::string aux;

    do{
        system("cls");
        std::cout << "% MEGATRON3000\n";
        std::cout << "   Welcone to MEGATRON 3000\n";
        std::cout << "& Agregar tabla" << (opcion == 0 ? " <-" : "") << std::endl;
        std::cout << "& Modificar Tabla" << (opcion == 1 ? " <-" : "") << std::endl;
        std::cout << "& Consulta" << (opcion == 2 ? " <-" : "") << std::endl;
        std::cout << "& Quit" << (opcion == 3 ? " <-" : "") << std::endl;

        tecla = _getch();

        // Procesar la tecla
        if (tecla == 72) { // Tecla de flecha arriba
            opcion = (opcion > 0) ? opcion - 1 : 3;
        } else if (tecla == 80) { // Tecla de flecha abajo
            opcion = (opcion < 3) ? opcion + 1 : 0;
        } else if (tecla == 13) { // Tecla Enter
            // Ejecutar la opción seleccionada
            system("cls");
            std::cout << "Seleccionaste la opcion " << opcion << std::endl;
            break; // Salir del bucle
        }

    }while(aux != "quit");

    return 0;
}

/*
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    // Abre el archivo en modo lectura
    std::ifstream file("Titanic control 2024 - Hoja 1.csv");

    // Verifica si el archivo se abrió correctamente
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    std::string line;
    // Lee cada línea del archivo
    while (std::getline(file, line)) {
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;
        // Divide la línea en campos separados por comas
        while (std::getline(ss, field, ',')) {
            // Agrega el campo al vector de campos
            fields.push_back(field);
        }
        // Procesa los campos como desees, aquí puedes imprimirlos
        for (const auto& f : fields) {
            std::cout << f << " ";
        }
        std::cout << std::endl;
    }

    // Cierra el archivo
    file.close();

    return 0;
}


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
int main() {

    std::string tabla = "ssdf.csv";
    std::string nombre = tabla.substr(tabla.length()-4);

    std:::cou
}
//*/