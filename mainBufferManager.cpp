#include <iostream>
#include <string>
#include <conio.h>
#include <limits>
#include "Disco.h"
#include "StorageManager.h"
#include "BufferManager.h"

/*
int main() {
    StorageManager m;

    DiscoDuro a(4,32,50,50);

    int opcion = 0;
    char tecla;
    do {
        system("cls");
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
//*/
//---------------------------FUNCIONAMIENTO DEL BUFFER MANAGER---------------------------


void bufferOperation(int pageID, const char& operation, BufferManager& manager) {
    try {
        if (operation == 'W' || operation == 'w') {
            manager.requestPage(pageID,operation);
            std::cout << "Pagina " << pageID << " escrita y marcada como sucia.\n";
        }
        else if (operation == 'L' || operation == 'l') {
            manager.requestPage(pageID,operation);
            std::cout << "Pagina " << pageID << " leida.\n";
        }
        //manager.releasePage(pageID);  // Despinnea la página independientemente de la operación para liberar el recurso
    }
    catch (const std::exception& e) {
        std::cout << "Error al procesar la pagina: " << e.what() << "\n";
    }
}
void releasePageFromMemory(int pageID, BufferManager& manager) {
        manager.releasePage(pageID);

}
void pinear(int pageID, BufferManager& manager) {
    manager.PinearPagina(pageID);
}


void displayMenu(BufferManager& manager) {
    int choice, pageID;
    char operation;

    while (true) {
        std::cout << "\n########  GESTION DE MEMORIA  ########\n";
        std::cout << "1. Solicitar pagina (Lectura/Escritura)\n";

        std::cout << "2. Imprimir tabla de paginas\n";
        std::cout << "3. Liberar pagina\n";
        std::cout << "4. Pinear pagina\n";
        std::cout << "5. Mostrar requerimientos\n";
        std::cout << "6. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Ingrese ID de pagina: ";
            std::cin >> pageID;
            std::cout << "Ingrese operacion (L para leer, W para escribir): ";
            std::cin >> operation;
            bufferOperation(pageID, operation, manager);
            break;
        case 2:
            manager.printPageTable();
            break;
        case 3:
            std::cout << "Ingrese ID de pagina: ";
            std::cin >> pageID;
            releasePageFromMemory(pageID, manager);
            break;
        case 4:
            std::cout << "Ingrese ID de pagina: ";
            std::cin >> pageID;
            pinear(pageID, manager);
            break;
        case 5:
            manager.PrintRequest();
            break;
        case 6:
            std::cout << "Saliendo del programa...\n";
            return;
        default:
            std::cout << "Opcion no valida. Intente de nuevo.\n";
        }
    }
}

int main() {
    int num;
    std::cout << "cuantas paginas quiere: ";
    std::cin>>num;
    BufferManager bufferManager(num, 40);  // Suponiendo un tamaño del buffer de 5 frames
    displayMenu(bufferManager);
    return 0;
}
//*/