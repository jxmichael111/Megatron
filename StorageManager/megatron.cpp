#include "megatron.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

Megatron::Megatron() : controladorDisco(false, 0, 0, 0, 0, 0), gestor (0, 0) {

}
Megatron::Megatron(bool tipo, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque, int frames)
    : controladorDisco(tipo, nroPlatos, nroPistas, nroSectores, bytesxSector, sectoresxBloque), gestor(frames, bytesxSector*sectoresxBloque) {
    this->rutaEsquema = RUTA_BASE + std::string("/esquema.txt");
    
    std::ifstream archivo(rutaEsquema);
    if (!archivo.good()) {
        std::ofstream nuevoArchivo(rutaEsquema);
        if (nuevoArchivo.is_open()) {
            nuevoArchivo.close();
            std::cout << "El archivo ha sido creado." << std::endl;
        } else {
            std::cerr << "Error al crear el archivo." << std::endl;
        }
    }
    archivo.close();
}

void Megatron::createStructure() {
    controladorDisco.createStructureDisk();
}


// ----------- SETS ----------------------------------------------------
void Megatron::setEsquema(const std::string nombreEsquema){
    this->esquemaActual = nombreEsquema;
    controladorDisco.setSizeScheme(nombreEsquema);
}

// ----------- ESQUEMA -------------------------------------------------
void Megatron::crearEsquemaDesdeCsv(const std::string file, int cant){
    std::cout << "Ruta completa: " << RUTA_BASE + file + ".csv" << std::endl;
    std::ifstream readFile(RUTA_BASE + file + ".csv");
    std::ofstream writeEsquema(this->rutaEsquema, std::ios::app);
    std::string esquema;
    int i = 0;

    if (readFile.is_open()) {
        
        //CREAR ESQUEMA CON CABECERA
        std::cout << "\tCrear esquema por defecto? (S/N): ";
        char option;
        std::cin >> option; 

        std::string lineaArchivo;
        std::string cabeza;
        std::string tipo;

        esquema += file;

        std::getline(readFile, lineaArchivo);

        if (option == 'S' || option == 's') {
            /*std::cout << "\tSelecciona una opción:" <<std::endl;
            std::cout << "\t1. Para Titanic" << std::endl;
            std::cout << "\t2. Para Posts" << std::endl;
            std::cin >> option;
            if (option == '1') 
                esquema = "Titanic#PassengerId#int#Survived#bool#Pclass#char#Name#varchar(60)#Sex#varchar(6)#Age#int#SibSp#bool#Parch#char#Ticket#varchar(16)#Fare#varchar(10)#Cabin#varchar(5)#Embarked#char";
            else 
                esequema = "Posts .................................................................."*/
            esquema = "Titanic#PassengerId#int#Survived#bool#Pclass#char#Name#varchar(60)#Sex#varchar(6)#Age#int#SibSp#bool#Parch#char#Ticket#varchar(16)#Fare#varchar(10)#Cabin#varchar(5)#Embarked#char";
        } else {            
            while (lineaArchivo[i] != '\0') {
                if (lineaArchivo[i] == ',') {
                    std::cout << "\tIndica el tipo de dato para '" << cabeza << "': " ;
                    std::cin >> tipo;   
                    esquema +=  + "#" + cabeza + "#" + tipo;
                    cabeza.clear();
                }
                else {
                    cabeza.push_back(lineaArchivo[i]);
                }
                i++;
            }
        }

        writeEsquema << esquema << std::endl;
        writeEsquema.close();
        

        setEsquema(esquema);

        //CREAR RELACIÓN CON CONTENIDO
        std::string relacionArchivo;
        relacionArchivo = crearArchivoRelacion(file);
        std::ofstream writeRelacion(relacionArchivo);

        if (cant == 0) {
            while (std::getline(readFile, lineaArchivo)) {
                //SET SI ES LONGITUD FIJA O VARIABLE      
                //controladorDisco.useLongitudFija(lineaArchivo);
                controladorDisco.insertar(lineaArchivo);
            }
        } else {
            int count = 0;
            while (std::getline(readFile, lineaArchivo)) { 
                //SET SI ES LONGITUD FIJA O VARIABLE
                //controladorDisco.useLongitudFija(lineaArchivo);
                controladorDisco.insertar(lineaArchivo);

                // CONTAR LÍNEAS
                count++;
                if (count == cant) 
                    break;
            }
        }

        writeRelacion.close();
        readFile.close();

        std::cout << "Esquema agregado y relacion creada exitosamente\n";
    } else {
        std::cerr << "Error al abrir el archivo." << std::endl;
    }
}

void Megatron::agregarEsquemaManual() {
    std::ofstream archivo(this->rutaEsquema);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string esquema;
    std::string respuesta;
    int contador = 1;

    std::cout << "Ingrese el NOMBRE del esquema: ";
    std::getline(std::cin, respuesta);
    esquema += respuesta + "#";

    std::cout << "Ingrese el NOMBRE del atributo " << contador << ": ";
    std::getline(std::cin, respuesta);
    esquema += respuesta + "#";
    contador++;

    std::cout << "Ingrese el TIPO del atributo '" << respuesta << "': ";
    std::cin >> respuesta;
    esquema += respuesta + "#";

    while (true) {
        std::cout << "Ingrese el NOMBRE del atributo " << contador << " (escriba '0' si desea salir): ";
        std::getline(std::cin, respuesta);
        esquema += respuesta + "#";
        contador++;

        if(respuesta == "0") break;
        std::cout << "Ingrese el TIPO del atributo '" << respuesta << "': ";
        std::cin >> respuesta;
        esquema += respuesta + "#";
    }
    archivo << esquema << std::endl;
    archivo.close();

    setEsquema(esquema);
}

/*void Megatron::eliminarEsquema(std::string lineaActual) {
    std::ofstream archivo(this->nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    std::getline(archivo, lineaActual);
    while (std::getline(archivoEntrada, lineaActual)) {
        if (lineaActual != lineaAEliminar) {
            archivoSalida << lineaActual << std::endl; 
        }
    }

    archivo.close();
}*/

// ------------ RELACIONES ----------------------------------------------

void Megatron::ingresarRelacionManual(bool tipo) {
    char rpta = 'S';
    std::string lineaEsquema;
    std::string encabezado;
    std::string tipoDato;
    std::string dato;

    /*if (!tipo) {
        this -> esquemaActual = crearArchivoRelacion(RUTA_BASE + this->esquemaActual + ".txt");
    }*/

    std::ofstream writeFile(RUTA_BASE + this->esquemaActual + ".txt", std::ios::app);
    if (!writeFile.is_open()) {
        std::cerr << "Error al abrir el archivo de relación\n";
        return;
    }
    std::cout << rutaEsquema << std::endl;
    std::ifstream readEsquema(this->rutaEsquema);
    if (!readEsquema.is_open()) {
        std::cerr << "Error al abrir el archivo de esquema\n";
        writeFile.close(); 
        return;
    }
    std::getline(readEsquema, lineaEsquema);
    std::cout << lineaEsquema << std::endl;

    while (std::getline(readEsquema, lineaEsquema)) {
        std::istringstream esquema_ss(lineaEsquema);
        std::getline(esquema_ss, encabezado, '#');
        

        std::cout << "Encabezado:" << encabezado << std::endl;
        std::cout << "Encabezado:" << this->esquemaActual << std::endl;
        if (encabezado == this->esquemaActual) {
            bool esPrimeraEntrada = true;
            while (std::getline(esquema_ss, encabezado, '#') && std::getline(esquema_ss, tipoDato, '#')) {
                std::cout << "Ingrese el dato para '" << encabezado << "': ";
                std::getline(std::cin, dato);
                if (esPrimeraEntrada) {
                    writeFile << dato;
                    esPrimeraEntrada = false;
                    std::getline(std::cin >> std::ws, dato);
                } else {
                    writeFile << "#" + dato;
                }
                

                std::cout << "Archivo llenado exitosamente\n";
            }
            writeFile << std::endl;
            break;
        }
    }
    readEsquema.close();
    writeFile.close();
}

void Megatron::ingresarDesdeArchivoCsv(const std::string file, int cant) {
    std::string lineaEsquema;
    std::string lineaArchivo;
    int i = 0;
    

    std::ofstream writeRelacion(RUTA_BASE + this->esquemaActual, std::ios::app);
    std::ifstream readFile(RUTA_BASE + this->rutaEsquema);
    std::ifstream archivo(RUTA_BASE + file);

    if (cant == 0) {
        while(std::getline(readFile, lineaArchivo)) {
            std::string atributo;
            std::string registro;
            bool is_string = false;
            i = 0;
            
            for (char c : lineaArchivo) {
                if (c == '"') {
                    is_string = !is_string;
                } else if (c == ',') {
                    if (is_string) {
                        atributo.push_back(c);
                    } else {
                        if (!atributo.empty()) {
                            registro += atributo + "#";
                            atributo.clear();
                        } else {
                            registro += "NULL#"; 
                        }
                    }
                } else {
                    atributo.push_back(c);
                }
            }

            // llenarSectorLongitudFija(lineaArchivo);
            
            registro += atributo;
            writeRelacion << registro << std::endl;         
        }
    }
    else {
        for(int i = 0; i < cant; i++){
            std::getline(readFile, lineaArchivo);
            std::string atributo;
            std::string registro;
            bool is_string = false;
            i = 0;
            
            for (char c : lineaArchivo) {
                if (c == '"') {
                    is_string = !is_string;
                } else if (c == ',') {
                    if (is_string) {
                        atributo.push_back(c);
                    } else {
                        if (!atributo.empty()) {
                            registro += atributo + "#";
                            atributo.clear();
                        } else {
                            registro += "NULL#"; 
                        }
                    }
                } else {
                    atributo.push_back(c);
                }
            }
            registro += atributo;
            writeRelacion << registro << std::endl; 
        }
    }
    writeRelacion.close();
    readFile.close();
}


// ------------- EXTRAS --------------------------------------------------

std::string Megatron::crearArchivoRelacion(std::string Esquema) {
    std::cout << "Creando relacion #" << Esquema << "...." << std::endl;

    std::string nombreRelacion = RUTA_BASE + Esquema + ".txt";
    std::ofstream archivo(nombreRelacion);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo " << nombreRelacion << std::endl;
        return "";
    }

    archivo.close();
    return nombreRelacion;
}
        
/*bool Megatron::eliminarRegistro(std::string query) {
    std::string nombreArchivo = this->esquemaActual;
    std::ifstream archivoIn(RUTA_BASE + this->esquemaActual + ".txt");
    if (!archivoIn.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return false;
    }

    std::ofstream archivoTemp("temp.txt");
    if (!archivoTemp.is_open()) {
        std::cerr << "Error al crear el archivo temporal." << std::endl;
        archivoIn.close();
        return false;
    }

    std::string linea;
    bool lineaEliminada = false;
    while (std::getline(archivoIn, linea)) {
        trim(linea);

        if (linea.find(query) != std::string::npos) {
            std::cout << "Se elimino la línea: " << linea << std::endl;
            lineaEliminada = true;
        } else {
            archivoTemp << linea << std::endl; // Escribir la línea en el archivo temporal
        }
    }

    // Cerrar archivos
    archivoIn.close();
    archivoTemp.close();

    // Eliminar el archivo original y renombrar el temporal si se realizó alguna eliminación
    if (lineaEliminada) {
        if (std::remove(nombreArchivo.c_str()) != 0) {
            std::cerr << "Error al eliminar el archivo original." << std::endl;
            return false;
        }
        if (std::rename("temp.txt", nombreArchivo.c_str()) != 0) {
            std::cerr << "Error al renombrar el archivo temporal." << std::endl;
            return false;
        }
    } else {
        std::cout << "No se encontró ninguna línea para eliminar." << std::endl;
        std::remove("temp.txt"); 
    }

    std::cout << "Operación completada correctamente." << std::endl;
    return true;    
}*/


void Megatron::menuDisco() {
    int option;

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
                controladorDisco.showInformation();

                break;                
            }
            case 2: {
                std::cout << "\tIndique el # de bloque: ";
                int bloque;
                std::cin >> bloque;

                controladorDisco.printBlockHeapFile(bloque);

                break;
            }
            case 3: {
                std::cout << "\tIndique el # de bloque: ";
                int bloque;
                std::cin >> bloque;

                controladorDisco.showBlockContent(bloque);

                break;
            }
            case 4: {
                controladorDisco.showFullHeapFile();
                
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

                controladorDisco.showSectorContent(plato, superficie, pista, sector);
                
                break;                
            }
            default: {
                std::cout << "Opcion no valida" << std::endl;
                break;
            }
        };
        
    }  while (option != 0);
}

void releasePageFromMemory(int pageID, BufferManager& manager) {
        manager.releasePage(pageID);

}
void pinear(int pageID, BufferManager& manager) {
    manager.PinearPagina(pageID);
}

void Megatron::menuBuffer() {   
    int choice, pageID;
    char operation;
    do {
        std::cout << "\n########  GESTION DE MEMORIA  ########\n";
        std::cout << "1. Solicitar pagina (Lectura/Escritura)\n";
        std::cout << "2. Imprimir tabla de paginas\n";
        std::cout << "3. Liberar pagina\n";
        std::cout << "4. Pinear pagina\n";
        std::cout << "5. Mostrar requerimientos\n";
        std::cout << "6. Mostrar data\n";
        std::cout << "0. Regresar\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> choice;
        switch (choice) {
        case 1:
            std::cout << "Ingrese ID de pagina: ";
            std::cin >> pageID;
            std::cout << "Ingrese operacion (L para leer, W para escribir): ";
            std::cin >> operation;
            try {
                if (operation == 'W' || operation == 'w') {
                    gestor.requestPage(pageID, operation, controladorDisco.readBlockToVector(pageID), &controladorDisco);
                    std::cout << "Pagina " << pageID << " escrita y marcada como sucia.\n";
                }
                else if (operation == 'L' || operation == 'l') {
                    gestor.requestPage(pageID,operation, controladorDisco.readBlockToVector(pageID), &controladorDisco);
                    std::cout << "Pagina " << pageID << " leida.\n";
                }
                //manager.releasePage(pageID);  // Despinnea la página independientemente de la operación para liberar el recurso
            }
            catch (const std::exception& e) {
                std::cout << "Error al procesar la pagina: " << e.what() << "\n";
            }
            break;
        case 2:
            gestor.printPageTable();
            break;
        case 3:
            std::cout << "Ingrese ID de pagina: ";
            std::cin >> pageID;
            gestor.releasePage(pageID);
            break;
        case 4:
            std::cout << "Ingrese ID de pagina: ";
            std::cin >> pageID;
            gestor.PinearPagina(pageID);
            break;
        case 5:
            gestor.PrintRequest();
            break;
        case 6:
            std::cout << "Ingrese ID de pagina: ";
            std::cin >> pageID;
            gestor.ViewPagina(pageID);
            return;
        case 7:
            std::cout << "Saliendo del programa...\n";
            return;
        default:
            std::cout << "Opcion no valida. Intente de nuevo.\n";
        }
    }  while (choice != 0);
}

void Megatron::cargarConfiguracion() {
    controladorDisco.recuperarHeapFile();

}
void Megatron::guardarConfiguracion() {
    controladorDisco.guardarHeapFile();
    controladorDisco.guardarStruct();
}

void Megatron::menuBPTree(){
    controladorDisco.MenuTree();
}

/*~Megatron() {
    delete[] arreglo_encabezados;
    delete[] arreglo_tipoDato;
}*/


