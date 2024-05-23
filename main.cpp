#include <iostream>
#include <string>
#include <conio.h>
#include <limits>
#include "Disco.h"
#include "StorageManager.h"

int main() {
    StorageManager m;

    DiscoDuro a(8,128,300,500);

    int opcion = 0;
    char tecla;
    do {
        //*
        system("cls");
        //*/
        std::cout << "% MEGATRON3000\n";
        std::cout << "   Welcone to MEGATRON 3000\n";
        std::cout << "& Agregar tabla" << (opcion == 0 ? " <-" : "") << std::endl;
        std::cout << "& Leer tabla" << (opcion == 1 ? " <-" : "") << std::endl;
        std::cout << "& Consulta" << (opcion == 2 ? " <-" : "") << std::endl;
        std::cout << "& Quit" << (opcion == 3 ? " <-" : "") << std::endl;

        tecla = _getch();

        // Procesar la tecla
        if (tecla == 72) { // Tecla de flecha arriba
            opcion = (opcion > 0) ? opcion - 1 : 3;
        } else if (tecla == 80) { // Tecla de flecha abajo
            opcion = (opcion < 3) ? opcion + 1 : 0;
        } else if (tecla == 13 && opcion == 0) { //agregar tabla
            system("cls");
            std::cout << "% MEGATRON3000\n";
            m.AgregarTabla();
        } else if (tecla == 13 && opcion == 1) { //  leer tabla
            system("cls");
            std::string b;
            int c;
            std::cout << "Ingresa el archivo a leer" << std::endl;
            std::cin >> b;
            std::cout << "Numero de registros a leer" << std::endl;
            std::cin >> c;
            std::cout << "% MEGATRON3000\n";
            m.LeerTabla(b,c);
        } else if (tecla == 13 && opcion == 2) { // consulta
            char b;
            system("cls");
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            string a;
            getline(cin, a);
            m.parser(a);
            std::cout << "desea salir | presione N/n?" <<std::endl;
            std::cin >> b;
        } else if (tecla == 13 && opcion == 3) {  // quit 
            system("cls");
            std::cout << "% MEGATRON3000\n";
            std::cout << "CERRANDO MEGATRON3000 ....\n";
            opcion = -5;
        }

    } while(opcion != -5);

    return 0;
}

    