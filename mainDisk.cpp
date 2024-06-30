#include "diskManager.h"

#include <iostream>
#include <string>

void menuDisco() {
    DiskManager disk(true, 2, 2, 4, 100, 2);
    int option;

    int response;

    disk.createStructureDisk();


    std::string esquema = "PassengerId#int#Survived#int#Pclass#int#Name#varchar(20)#Sex#varchar(8)#Age#int#SibSp#int#Parch#int#Ticket#varchar(10)#Fare#float#Cabin#varchar(4)#Embarked#char";
    disk.setSizeScheme(esquema);

    //std::string lineaArchivo = "9,1,3,"Andre",female,27,0,2,347742,11.1333,,S";
    std::ifstream file(RUTA_BASE + std::string("texto.txt"));
    std::string linea;
    getline(file, linea);
    
    

    do {
        std::cout << "\n\n*********************************************************" << std::endl;
        std::cout << "--------- MENU DISCO ----------" << std::endl;
        std::cout << "1. Mostrar informacion de disco" << std::endl; //HECHO
        std::cout << "2. Mostrar informacion de bloque" << std::endl; //HECHO
        std::cout << "3. Mostrar contenido de bloque" << std::endl; //HECHO
        std::cout << "4. Mostrar Heap File (Linked List)" << std::endl; // HECHO
        std::cout << "5. Mostrar contenido de sector por ubicacion" << std::endl; // HECHO
        std::cout << "0. Regresar" << std::endl; 
        std::cout << "\tIngresa una opcion: ";
        std::cin >> option;

        switch (option) {
            case 1: {
                disk.showInformation();
                disk.useLongitudFija(linea);

                break;                
            }
            case 2: {
                std::cout << "\tIndique el # de bloque: ";
                int bloque;
                std::cin >> bloque;

                disk.showBlockContent(bloque);

                break;
            }
            case 3: {
                std::cout << "\tIndique el # de bloque: ";
                int bloque;
                std::cin >> bloque;

                disk.printBlockHeapFile(bloque);

                break;
            }
            case 4: {
                disk.showFullHeapFile();
                
                break;                
            }
            case 5: {
                std::cout << "\n\tIndique el plato: ";
                int plato;
                std::cin >> plato;

                std::cout << "\tIndique la superficie (A | B): ";
                char superficie;
                std::cin >> superficie;

                std::cout << "\tIndique la pista: ";
                int pista;
                std::cin >> pista;

                std::cout << "\tIndique el sector: ";
                int sector;
                std::cin >> sector;
                
                std::cout << std::endl;

                disk.showSectorContent(plato, superficie, pista, sector);
                
                break;                
            }
            default: {
                std::cout << "Opcion no valida" << std::endl;
                break;
            }
        };
        
    }  while (option != 0);
}

int main() {
    
    menuDisco();

    return 0;
}