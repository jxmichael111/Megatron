#include <string>
#include "diskManager.h"

// ---------------------------Metodos para el BPlusTree---------------------------
void insertionMethod(BPTree* bPTree, int key, int NroBloque, int NroRegistro) {
    // indice denso por key
    // llena el arbol
    (bPTree)->insert(key, NroBloque, NroRegistro);
    std::cout << "Se inserto el key:" << key << std::endl;
 
    // serializa el arbol (para poder recuperarlo)
    (bPTree)->serialize("bptree.dat");
}

void printMethod(BPTree* bPTree) { 
    int opt;
    std::cout << "\n\t1.Arbol de niveles \n\t2. Forma secuencial\n";
    std::cin >> opt;

    std::cout << std::endl;
    if (opt == 1)
        bPTree->display(bPTree->getRoot());
    else
        bPTree->seqDisplay(bPTree->getRoot());
}

void deleteMethod(BPTree* bPTree,int tmp) {
    std::cout << "Del arbol, cual deseas borrar?: " << std::endl;
    bPTree->display(bPTree->getRoot());
 
    bPTree->removeKey(tmp);

    std::cout << "Arbol actualizado" << std::endl;
    bPTree->display(bPTree->getRoot());
}

std::pair<int, int> searchMethod(BPTree* bPTree, int idx) {
    // Llamar a la función search de BPTree y recibir el par resultante
    std::pair<int, int> result = bPTree->getUbicacionRegistro(idx);
    return result;
}

// ==========================================================================
void eliminarLinea(const std::string& archivoEntrada, const std::string& archivoSalida, int numLineaEliminar);
template <typename Cabecera>
void reemplazarCabecera(const std::string& archivo, const Cabecera& nuevaCabecera);
std::vector<int> splitStringToVector(const std::string& str, char delimiter);
std::string joinVectorToString(const std::vector<int>& vec, char delimiter);
std::string insertAndSort(const std::string& str, int num);
std::string removerPrimerElemento(const std::string& cadena);


DiskManager::DiskManager() : disco(0, 0, 0, 0, 0) {}

DiskManager::DiskManager(bool tipo, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque) : disco(nroPlatos, nroPistas, nroSectores, bytesxSector, sectoresxBloque), tipoLongitud(tipo), tipoCampo(nullptr), freeSpaceInicial(nullptr), freeSpaceFinal(nullptr), fullSpaceInicial(nullptr), fullSpaceFinal(nullptr) {
    this->tipoLongitud = tipo;
    this->platoAct = 1;
    this->superfAct = 'A';
    this->pistaAct = 1;
    this->sectorAct = 1;
    this->bloqueAct = 1;
    this->bPTree = new BPTree(7,7);
}

// =============================================  GENERAL  ==============================================================

void DiskManager::showInformation() {
    std::cout << "\t+-----------------------------+-------------------+" << std::endl;
    std::cout << "\t| Concepto                    | Cantidad          |" << std::endl;
    std::cout << "\t+-----------------------------+-------------------+" << std::endl;
    std::cout << "\t| Cantidad de platos          | " << std::setw(17) << disco.getCantidadPlatos() << " |" << std::endl;
    std::cout << "\t| Cantidad de pistas x plato  | " << std::setw(17) << disco.getCantidadPistas() << " |" << std::endl;
    std::cout << "\t| Cantidad de sectores x pista| " << std::setw(17) << disco.getCantidadSectores() << " |" << std::endl;
    std::cout << "\t| Cantidad de bytes x sector  | " << std::setw(17) << disco.getBytesxSector() << " |" << std::endl;
    std::cout << "\t| Cantidad total de bloques   | " << std::setw(17) << disco.getCantidadBloques() << " |" << std::endl;
    std::cout << "\t+-----------------------------+-------------------+" << std::endl;
}

void DiskManager::getBlockInformation() {
    std::cout << "------------------------------------------------------" << std::endl;
    disco.printTamañoTotal();
    std::cout << "La cantidad de bloques es: " << disco.getCantidadBloques() << std::endl;
    std::cout << "Tamanio del bloque: " << disco.getTamañoBloque() << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
}
void DiskManager::validarUbicacionActual() {    
    // Verificar si sectorAct excede la cantidad de sectores por pista
    if (this->sectorAct > disco.getCantidadSectores()) {
        this->sectorAct = 1;
        this->pistaAct++;
        std::cout << "Pista: " << this->pistaAct << std::endl;
        
        // Verificar si pistaAct excede la cantidad de pistas por superficie
        if (this->pistaAct > disco.getCantidadPistas()) {
            this->pistaAct = 1;
            this->superfAct++;

            std::cout << "Superficie: " << this->superfAct << std::endl;
            
            // Verificar si superfAct excede el número de superficies por plato
            if (this->superfAct > 'B') {
                this->superfAct = 'A';
                this->platoAct++;
                
                // Verificar si platoAct excede la cantidad de platos
                if (this->platoAct > disco.getCantidadPlatos()) {
                    std::cout << "No hay más espacio disponible en el disco" << std::endl;
                    // Lógica adicional para manejar el caso de disco lleno
                    this->platoAct = disco.getCantidadPlatos(); // Mantener platoAct en el valor máximo permitido
                }
            }
        }
    }
}

void DiskManager::createStructureDisk() {
    // * Crear file sectores
    std::string carpetaSectores = RUTA_BASE + std::string("/Sectores"); 
    fs::remove_all(carpetaSectores); // Remueve carpetas anteriores con el mismo nombre    
    fs::create_directories(carpetaSectores); // Crea la carpeta directorios

    for (int i = 0; i < disco.getTotalSectores(); i++) {
        std::string archivoSector = carpetaSectores + "/" + std::to_string(this->platoAct) + "-" + this->superfAct + "-" + std::to_string(this->pistaAct) + "-" + std::to_string(this->sectorAct) + ".txt";
        std::ofstream archivo(archivoSector);
        archivo << this->sectorAct << "#" << disco.getBytesxSector() << "##" << std::to_string(this->platoAct) + "," + std::string(1, this->superfAct) + "," + std::to_string(this->pistaAct) + "," + std::to_string(this->sectorAct) + "#";
        archivo << std::endl;
        archivo.close();

        this->sectorAct++;
        
        validarUbicacionActual();
    }

    this->platoAct = 1;
    this->superfAct = 'A';
    this->pistaAct = 1;
    this->sectorAct = 1;

    int sizeBlock;
    sizeBlock = disco.getTamañoBloque();;

    // *Crear los files de bloques
    std::string carpetaBloques = RUTA_BASE + std::string("/Bloques") ;
    fs::remove_all(carpetaBloques);
    fs::create_directories(carpetaBloques);

    for (int bloque = 1; bloque <= disco.getCantidadBloques(); ++bloque) {
        std::string archivoBloque = carpetaBloques + "/Bloque" + std::to_string(bloque) + ".txt";
        std::ofstream archivo(archivoBloque);
        if (!archivo.is_open()) {
            std::cerr << "Error al crear el archivo " << archivoBloque << '\n';
            continue;
        }
        
        std::string contenido = std::to_string(bloque) + "#" + std::to_string(disco.getTamañoBloque()) + "#";
        archivo << contenido << std::endl;
        archivo.close();

        std::vector<std::tuple<int, char, int, int, bool>> sectores;
        for (int sector = 1; sector <= disco.getCantidadSectoresxBloque(); ++sector) {
            sectores.emplace_back(this->platoAct, this->superfAct, this->pistaAct, this->sectorAct, false);
            std::cout << "Plato: " << this->platoAct << "; Superficie: " << this->superfAct << "; Pista: " << this->pistaAct << "; Sector: " << this->sectorAct << std::endl;
            this->sectorAct++;
            validarUbicacionActual();
        }

        insertBlocktoFreeHeapFile(bloque, disco.getTamañoBloque(), sectores);
    }

    this->platoAct = 1;
    this->superfAct = 'A';
    this->pistaAct = 1;
    this->sectorAct = 1;

    std::cout << "La estructura del disco ha sido creada exitosamente." << std::endl;
}

void DiskManager::actualizarSector() {
    this->sectorAct++;
    std::cout << "Actualizando al siguiente sector: " << sectorAct << std::endl;

    validarUbicacionActual();
}

void DiskManager::actualizarBloque() {
    this->bloqueAct++;
    std::cout << "Actualizando al siguiente bloque: " << bloqueAct << std::endl;

    validarUbicacionActual();
}

/*
    Funcion para mostrar contenido de un numBloque y un numRegistro
*/
void DiskManager::showBlockandRegisterContent(int numBloque, int NroRegistro) {
    std::string carpetaBloque = RUTA_BASE + std::string("Bloques/");
    std::string archivoBloque = carpetaBloque + std::string("Bloque") + std::to_string(numBloque) + ".txt";
    std::ifstream archivo(archivoBloque);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    for (int i=0; i < NroRegistro; i++){
        std::getline(archivo, linea);
    }
    std::cout << "Informacion del registro: " << linea << std::endl;

    archivo.close();
}

void DiskManager::showBlockContent(int numBloque) {
    std::string carpetaBloque = RUTA_BASE + std::string("Bloques/");
    std::string archivoBloque = carpetaBloque + std::string("Bloque") + std::to_string(numBloque) + ".txt";
    std::ifstream archivo(archivoBloque);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::cout << linea << std::endl;
    }

    archivo.close();
}

void DiskManager::showSectorContent(int plato, char superficie, int pista, int sector) {
    std::string carpetaSectores = RUTA_BASE + std::string("Sectores/");
    std::string archivoSector = carpetaSectores + std::to_string(plato) + "-" + superficie + "-" + std::to_string(pista) + "-" + std::to_string(sector) + ".txt";
    std::ifstream archivo(archivoSector);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::cout << linea << std::endl;
    }

    archivo.close();
}

void DiskManager::guardarStruct() {
    std::string filename = RUTA_BASE + std::string("discoInfo.txt");
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo para escribir." << std::endl;
        return;
    }

    file << disco.getCantidadPlatos() << std::endl;
    file << disco.getCantidadPistas() << std::endl;
    file << disco.getCantidadSectores() << std::endl;
    file << disco.getBytesxSector() << std::endl;
    file << disco.getCantidadBloques() << std::endl;
    file << disco.getCantidadSectoresxBloque() << std::endl;

    file.close();
    std::cout << "Informacion del disco guardada correctamente." << std::endl;
}


// ================ CONEXIÓN CON BUFFER ============

void DiskManager::insertar(std::string linea) {
    if(this->tipoLongitud) {
        //useLongitudVariable(linea);
    } else {
        useLongitudFija(linea);
    }
}

void DiskManager::actualizar(int numBloque, std::string linea, int numRegistro) {
    if(this->tipoLongitud) {
        //actualizarLineaLongitudFija(numBloque, linea, numRegistro);
    } else {
        actualizarLineaLongitudFija(numBloque, linea, numRegistro);
    }
}

/* Pagina -> Requerimientos -> Disco

    
    Registro 1
    Registro 2

    Registro 4
    
    L L W L W L
    
    W -> conte, c


    numLinea:
        Delete - Actualizar : recuperas del arbol
        Inserción = -1  
        -1 si no existe

        
                

    vector<string> searchKeysDeletedRegisters;

    
*/

void DiskManager::eliminar(int numBloque, int numRegistro,int claveBusqueda){ //recibe la clave de busqueda
    if(this->tipoLongitud) {
        //useLongitudVariable(numBloque, numRegistro);
    } else {
        
        // search del num para el path (num bloque, num registro) 
        // nos devuelve ---->  <NroBloque NroLinea>
        // eliminamos en el disco
        eliminarLineaLongitudFija(numBloque, numRegistro);
        // eliminar en el bplus
        deleteMethod(bPTree, claveBusqueda);
    }
}

/*
    Cargar un bloque a una página: Carga el contenifo de un file (blqoue) a un vector (página).
*/
std::vector<std::string> DiskManager::readBlockToVector(int numBloque) { 
    std::vector<std::string> registros;
    Nodo* bloque = searchBlockHeapFile(numBloque);

    std::string carpetaBloque = RUTA_BASE + std::string("Bloques/");
    std::string archivoBloque = carpetaBloque + "Bloque" + std::to_string(numBloque) + ".txt";
    std::ifstream file(archivoBloque);

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << archivoBloque << std::endl;
        return registros;
    }

    std::string line;

    //Espacio total y libre
    line = std::to_string(disco.getTamañoBloque()) + " " + std::to_string(bloque->espacioLibre);

    registros.push_back(line);

    std::getline(file, line); // Ignorar la cabecera
    while (std::getline(file, line)) {
        std::string info;
        if (!this->tipoLongitud) {
            for (int i = 0; i < this->numCampos; ++i) {
                info += std::to_string(this->tipoCampo[i]);
                if (i != this->numCampos-1)
                    info += " ";
            }
            info += "|";
            registros.push_back(info + line);
        } else {
            registros.push_back(line);
        }
    }

    file.close();
    return registros;
}

/*
    Devuelve una tupla, el num bloque y el num de linea
    Se utiliza para el arbol Bplus
*/

std::pair<int, int> DiskManager::buscarID(const std::string& id) { //Soicitado por el árbol
    for (int i = 0; i < disco.getCantidadBloques(); ++i) {
        std::vector<std::string> registros = readBlockToVector(i);
        for (size_t j = 0; j < registros.size(); ++j) {
            if (registros[j].find(id) != std::string::npos) {
                std::cout << "Registro encontrado en el BLOQUE: " << i << ", LINEA: " << j << std::endl;
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1); // ID no encontrado
}

/*
    Recibe un vector(página) del BufferManager para guardar en el disco. Maneja inserciones y modifcaciones.
*/
void DiskManager::insertPagetoBlock(std::vector<std::string>& pagina, int numPag, int claveBusqueda) {
    Nodo* bloque = searchBlockHeapFile(numPag);
    
    std::string carpetaBloque = RUTA_BASE + std::string("Bloques/");
    std::string archivoBloque = carpetaBloque + "Bloque" + std::to_string(numPag) + ".txt";
    std::ofstream file(archivoBloque, std::ios::trunc); // Abre el archivo en modo trunc (borra contenido si existe)

    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo " << archivoBloque << " para escritura.\n";
        return;
    }

    // Procesa la segunda línea para actualizar el espacio libre
    if (pagina.size() > 1) {
        file << pagina[1] << std::endl;

        std::stringstream ss(pagina[1]);
        std::string token;
        std::getline(ss, token, '#'); // Ignorar el identificador
        std::getline(ss, token, '#');
        bloque->espacioLibre = std::atoi(token.c_str());
    }

    std::string registrosLibres;
    int i = 1;

    for (auto it = bloque->sectores.begin(); it != bloque->sectores.end(); ++it) {
        bool estado = std::get<4>(*it); // Obtener el estado del sector
        if (!estado) {
            auto tupla = it;
            this->platoAct = std::get<0>(*tupla);
            this->superfAct = std::get<1>(*tupla);
            this->pistaAct = std::get<2>(*tupla);
            this->sectorAct = std::get<3>(*tupla);

            bool found = true; // Se encontró un sector con estado false
            break;
        }
    }

    for (const std::string& linea : pagina) {
        if (linea.find('$') != std::string::npos && linea.size() == 1) {
            registrosLibres = std::to_string(i) + ",";
            std::string registroVacio(this->longitudRegistro, ' ');
            file << registroVacio << std::endl;

            // LLENAR SECTOR
            sectorFillLongitudFija(registroVacio, i, bloque);

            // ARBOL
            // deleteMethod(bPTree, claveBusqueda);
        } else if (linea.find('$') != std::string::npos) {
            std::string nuevaLinea = linea.substr(linea.find('$')); // Obtener la subcadena desde el '$'
            file << nuevaLinea << std::endl;

            // LLENAR SECTOR
            sectorFillLongitudFija(nuevaLinea, i, bloque);

        } else if (linea.find('+') != std::string::npos) {
            std::string nuevaLinea = linea.substr(linea.find('+')); // Obtener la subcadena desde el '+'
            file << nuevaLinea << std::endl;

            // LLENAR SECTOR
            sectorFillLongitudFija(nuevaLinea, -1, bloque);
        } else {
            file << linea << std::endl;
        }
        i++;
    }

    CabeceraBloque nuevaCabecera;
    nuevaCabecera.identificador = bloque->numeroBloque;
    nuevaCabecera.espacioDisponible = bloque->espacioLibre;
    nuevaCabecera.espaciosLibres = registrosLibres;
    reemplazarCabecera(archivoBloque, nuevaCabecera);

    file.close();
}



// ============================================ HEAP FILE ===================================================

void DiskManager::insertBlocktoFreeHeapFile(int numBloque, int espLibre, const std::vector<std::tuple<int, char, int, int, bool>>& sectores) {
    Nodo* nuevoNodo = new Nodo(numBloque, espLibre);
    nuevoNodo->sectores = sectores;

    if (!this->freeSpaceInicial) {
        this->freeSpaceInicial = this->freeSpaceFinal = nuevoNodo;
        this->bloqueAct = this->freeSpaceInicial->numeroBloque;
        auto it = this->freeSpaceInicial->sectores.begin();
        if (it != this->freeSpaceInicial->sectores.end()) {
            this->platoAct = std::get<0>(*it);
            this->superfAct = std::get<1>(*it);
            this->sectorAct = std::get<2>(*it);
            this->pistaAct = std::get<3>(*it);
        }
    } else {
        this->freeSpaceFinal->next = nuevoNodo;
        nuevoNodo->prev = this->freeSpaceFinal;
        this->freeSpaceFinal = nuevoNodo;
    }
}

void DiskManager::insertBlocktoFullHeapFile(int numBloque, int espLibre, const std::vector<std::tuple<int, char, int, int, bool>>& sectores) {
    Nodo* nuevoNodo = new Nodo(numBloque, espLibre);
    nuevoNodo->sectores = sectores;

    if (!this->fullSpaceInicial) {
        this->fullSpaceInicial = this->fullSpaceFinal = nuevoNodo;
    } else {
        this->fullSpaceFinal->next = nuevoNodo;
        nuevoNodo->prev = this->fullSpaceFinal;
        this->freeSpaceFinal = nuevoNodo;
    }
}

void DiskManager::showFullHeapFile() {
    std::cout << "============ LISTA DE BLOQUES VACIOS ===============" << std::endl;
    Nodo* actualfree = this->freeSpaceInicial;
    while (actualfree) {
        printBlockInformation(actualfree);
        actualfree = actualfree->next;
    }
    if(!this->freeSpaceInicial)
        std::cout << "La lista de Bloques vacios esta vacia" << std::endl;
    
    std::cout << "============ LISTA DE BLOQUES LLENOS ===============" << std::endl;
    Nodo* actualfull = this->fullSpaceInicial;
    while (actualfull) {
        printBlockInformation(actualfull);
        actualfull = actualfull->next;
    }
    if(!this->fullSpaceInicial)
        std::cout << "La lista de Bloques llenos esta vacia" << std::endl;
}

void DiskManager::printBlockInformation(Nodo* nodo) {
    std::cout << "\tBloque: " << nodo->numeroBloque
                << ", Espacio Libre: " << nodo->espacioLibre
                << ", Sectores: ";

    for (const auto& tupla : nodo->sectores) {
        int plato = std::get<0>(tupla);
        char superficie = std::get<1>(tupla);
        int pista = std::get<2>(tupla);
        int idSector = std::get<3>(tupla);
        bool estado = std::get<4>(tupla);

        std::cout << "(" << plato << ", " << superficie << ", " << pista << ", " << idSector << ", " << estado << ") ";
    }
    std::cout << std::endl;
}

void DiskManager::printBlockHeapFile(int numBloque) {
    Nodo* actual = this->freeSpaceInicial;
    while (actual) {
        if (actual->numeroBloque == numBloque) {
            printBlockInformation(actual);
            return;
        }
        actual = actual->next;
    }

    actual = this->fullSpaceInicial;
    while (actual) {
        if (actual->numeroBloque == numBloque) {
            printBlockInformation(actual);
            return;
        }
        actual = actual->next;
    }

    std::cout << "Bloque con numero " << numBloque << " no encontrado." << std::endl;
}

Nodo* DiskManager::searchFreeSpace(int numBloque) {
    Nodo* actual = this->freeSpaceInicial;
    while (actual) {
        std::cout << "--> Visitando bloque " << actual->numeroBloque << " en heap file (Free)" << std::endl;
        if (actual->numeroBloque == numBloque) {
            return actual;
        }
        actual = actual->next;
    }
    return nullptr;  // Devuelve nullptr si no encuentra el bloque
}

Nodo* DiskManager::searchFullSpace(int numBloque) {
    Nodo* actual = this->fullSpaceInicial;
    while (actual) {
        std::cout << "Visitando bloque " << actual->numeroBloque << " en heap file (Full)" << std::endl;
        if (actual->numeroBloque == numBloque) {
            return actual;
        }
        actual = actual->next;
    }
    return nullptr;  // Devuelve nullptr si no encuentra el bloque
}

Nodo* DiskManager::searchBlockHeapFile(int numBloque) {
    Nodo* nodo = searchFreeSpace(numBloque);
    if (nodo) {
        return nodo;
    }
    
    nodo = searchFullSpace(numBloque);
    if (nodo) {
        return nodo;
    }

    std::cout << "Bloque con numero " << numBloque << " no encontrado." << std::endl;
    return nullptr;  // Devuelve nullptr si no encuentra el bloque
}

void DiskManager::decreaseSpaceofBlock(int numBloque) {
    Nodo* bloque = searchBlockHeapFile(numBloque);

    if (!bloque) {
        std::cout << "Bloque con numero " << numBloque << " no encontrado." << std::endl;
        return;
    }

    bloque->espacioLibre -= longitudRegistro;

    std::cout << " -- Bloque " << bloque->numeroBloque << " con espacio libre reducido a " << bloque->espacioLibre << std::endl;

    if (bloque->espacioLibre < longitudRegistro) {
        moveBlockFreeToFull(bloque);
        if (this->freeSpaceInicial) {
            this->bloqueAct = this->freeSpaceInicial->numeroBloque;
            auto it = this->freeSpaceInicial->sectores.begin();
            if (it != this->freeSpaceInicial->sectores.end()) {
                this->sectorAct = std::distance(this->freeSpaceInicial->sectores.begin(), it); // Use the index as sector ID
                this->platoAct = std::get<0>(*it);
                this->superfAct = std::get<1>(*it);
                this->pistaAct = std::get<2>(*it);
            }
        }
    }
}

void DiskManager::increaseSpaceofBlock(int numBloque) {
    Nodo* bloque = searchBlockHeapFile(numBloque);

    if (!bloque) {
        std::cout << "Bloque con numero " << numBloque << " no encontrado." << std::endl;
        return;
    }

    bloque->espacioLibre += longitudRegistro;

    std::cout << "++ Bloque " << bloque->numeroBloque << " con espacio libre incrementado a " << bloque->espacioLibre << std::endl;

    if (searchFullSpace(bloque->numeroBloque) && bloque->espacioLibre >= longitudRegistro) {
        moveBlockFullToFree(bloque);
        if (this->freeSpaceInicial) {
            this->bloqueAct = this->freeSpaceInicial->numeroBloque;
            auto it = this->freeSpaceInicial->sectores.begin();
            if (it != this->freeSpaceInicial->sectores.end()) {
                this->sectorAct = std::distance(this->freeSpaceInicial->sectores.begin(), it); // Use the index as sector ID
                this->platoAct = std::get<0>(*it);
                this->superfAct = std::get<1>(*it);
                this->pistaAct = std::get<2>(*it);
            }
        }
    }
}

void DiskManager::deleteBlockHeapFile(Nodo*& head, Nodo*& tail, Nodo* nodo) {
    if (!nodo) return;

    if (nodo->prev) {
        nodo->prev->next = nodo->next;
    } else {
        head = nodo->next;
    }

    if (nodo->next) {
        nodo->next->prev = nodo->prev;
    } else {
        tail = nodo->prev;
    }

    nodo->prev = nullptr;
    nodo->next = nullptr;
}

void DiskManager::moveBlockFreeToFull(Nodo* nodo) {
    deleteBlockHeapFile(this->freeSpaceInicial, this->freeSpaceFinal, nodo);

    // Insertar nodo al final de FullSpace
    if (!fullSpaceInicial) {
        fullSpaceInicial = fullSpaceFinal = nodo;
    } else {
        fullSpaceFinal->next = nodo;
        nodo->prev = fullSpaceFinal;
        fullSpaceFinal = nodo;
    }

    std::cout << "El bloque " << nodo->numeroBloque << " fue movido de la lista FREE a FULL" << std::endl;

    nodo->next = nullptr;
}

void DiskManager::moveBlockFullToFree(Nodo* nodo) {

    deleteBlockHeapFile(this->fullSpaceInicial, this->fullSpaceFinal, nodo);

    if (!this->freeSpaceInicial) {
        this->freeSpaceInicial = this->freeSpaceFinal = nodo;
    } else {
        nodo->next = this->freeSpaceInicial;
        this->freeSpaceInicial->prev = nodo;
        this->freeSpaceInicial = nodo;
    }

    std::cout << "El bloque " << nodo->numeroBloque << " fue movido de la lista FULL a FREE" << std::endl;

    nodo->prev = nullptr;
}

void DiskManager::vaciarHeapFile(Nodo*& head, Nodo*& tail) {
    Nodo* actual = head;
    while (actual) {
        Nodo* siguiente = actual->next;
        delete actual;
        actual = siguiente;
    }
    head = nullptr;
    tail = nullptr;
}

void DiskManager::vaciarHeapFile() {
    vaciarHeapFile(this->freeSpaceInicial, this->freeSpaceFinal);
    vaciarHeapFile(this->fullSpaceInicial, this->fullSpaceFinal);
}

void DiskManager::guardarHeapFile() {
    std::string filename = RUTA_BASE + std::string("heapFile.txt");
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error al abrir el archivo para escritura: " << filename << std::endl;
        return;
    }

    // Guardar bloques con espacio libre
    Nodo* actual = this->freeSpaceInicial;
    outFile << "FreeSpace" << std::endl;
    while (actual) {
        outFile << actual->numeroBloque << "#" << actual->espacioLibre;
        for (const auto& tupla : actual->sectores) {
            outFile << "#" << std::get<0>(tupla) << "," << std::get<1>(tupla) << "," << std::get<2>(tupla) << "," << std::get<3>(tupla) << "," << std::get<4>(tupla);
        }
        outFile << std::endl;
        actual = actual->next;
    }

    // Guardar bloques llenos
    actual = this->fullSpaceInicial;
    outFile << "FullSpace" << std::endl;
    while (actual) {
        outFile << actual->numeroBloque << "#" << actual->espacioLibre;
        for (const auto& tupla : actual->sectores) {
            outFile << "#" << std::get<0>(tupla) << "," << std::get<1>(tupla) << "," << std::get<2>(tupla) << "," << std::get<3>(tupla) << "," << std::get<4>(tupla);
        }
        outFile << std::endl;
        actual = actual->next;
    }

    outFile.close();
}

void DiskManager::recuperarHeapFile() {
    std::string filename = RUTA_BASE + std::string("heapFile.txt");
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error al abrir el archivo para lectura: " << filename << std::endl;
        return;
    }

    std::string line;
    Nodo* nuevoNodo;
    bool isFreeSpace = true;

    while (std::getline(inFile, line)) {
        if (line == "FreeSpace") {
            isFreeSpace = true;
            continue;
        } else if (line == "FullSpace") {
            isFreeSpace = false;
            continue;
        }

        std::istringstream ss(line);
        std::string token;
        std::getline(ss, token, '#');
        int numBloque = std::stoi(token);

        std::getline(ss, token, '#');
        int espLibre = std::stoi(token);

        std::vector<std::tuple<int, char, int, int, bool>> sectores;
        while (std::getline(ss, token, '#')) {
            std::istringstream ssToken(token);
            std::string sectorData;
            int plato, pista, idSector;
            char superficie;
            bool espacioSector;

            std::getline(ssToken, sectorData, ',');
            plato = std::stoi(sectorData);

            std::getline(ssToken, sectorData, ',');
            superficie = sectorData[0];

            std::getline(ssToken, sectorData, ',');
            pista = std::stoi(sectorData);

            std::getline(ssToken, sectorData, ',');
            idSector = std::stoi(sectorData);

            std::getline(ssToken, sectorData);
            espacioSector = (sectorData == "1");

            sectores.push_back(std::make_tuple(plato, superficie, pista, idSector, espacioSector));
        }

        nuevoNodo = new Nodo(numBloque, espLibre);
        nuevoNodo->sectores = sectores;

        if (isFreeSpace) {
            if (!this->freeSpaceInicial) {
                this->freeSpaceInicial = this->freeSpaceFinal = nuevoNodo;
                this->bloqueAct = this->freeSpaceInicial->numeroBloque;
                auto it = this->freeSpaceInicial->sectores.begin();
                if (it != this->freeSpaceInicial->sectores.end()) {
                    this->platoAct = std::get<0>(*it);
                    this->superfAct = std::get<1>(*it);
                    this->sectorAct = std::get<2>(*it);
                    this->pistaAct = std::get<3>(*it);
                }
            } else {
                this->freeSpaceFinal->next = nuevoNodo;
                nuevoNodo->prev = this->freeSpaceFinal;
                this->freeSpaceFinal = nuevoNodo;
            }
        } else {
            if (!this->fullSpaceInicial) {
                this->fullSpaceInicial = this->fullSpaceFinal = nuevoNodo;
            } else {
                this->fullSpaceFinal->next = nuevoNodo;
                nuevoNodo->prev = this->fullSpaceFinal;
                this->fullSpaceFinal = nuevoNodo;
            }
        }
    }

    inFile.close();
}


// ============================================= LONGITUD FIJA ==========================================================

/*
    * Calcula el tamaño de un registro por esquema (LONGITUD FIJA)
*/
void DiskManager::setSizeScheme(const std::string& esquema) {
    std::stringstream ss(esquema);
    std::string temp, campo, tipo;

    std::vector<std::string> campos;
    while (std::getline(ss, temp, '#')) {
        campos.push_back(temp);
    }

    if (campos.size() % 2 != 1) {
        std::cerr << "Esquema inválido." << std::endl;
        return;
    }

    int size = 0;
    int numCampos = (campos.size() - 1) / 2;
    tipoCampo = new int[numCampos];

    int i = 0;
    for (size_t j = 1; j < campos.size(); j += 2) {
        campo = campos[j];
        tipo = campos[j + 1];

        if (tipo == "int") {
            tipoCampo[i] = sizeof(int);
            size += sizeof(int);
        } else if (tipo == "char") {
            tipoCampo[i] = sizeof(char);
            size += sizeof(char);
        } else if (tipo == "float") {
            tipoCampo[i] = sizeof(float);
            size += sizeof(float);
        } else if (tipo == "double") {
            tipoCampo[i] = sizeof(double);
            size += sizeof(double);
        } else if (tipo == "bool") {
            tipoCampo[i] = sizeof(bool);
            size += sizeof(bool);
        } else if (tipo.substr(0, 7) == "varchar") {
            size_t pos1 = tipo.find('(');
            size_t pos2 = tipo.find(')');
            if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1 + 1) {
                int length = std::stoi(tipo.substr(pos1 + 1, pos2 - pos1 - 1));
                tipoCampo[i] = length * sizeof(char);
                size += length * sizeof(char);
                
            }
        }
        ++i;
    }
    this->numCampos = i;

    std::cout << "Tamaño del registro: " << size << " bytes" << std::endl;
    setLongitudRegistro(size);

    for (int j = 0; j < i; ++j) {
        std::cout << "Tamaño del tipo en el índice " << j << ": " << tipoCampo[j] << " bytes\n";
    }
}

void DiskManager::setLongitudRegistro(int longitud) {
    this->longitudRegistro = longitud;
}

void DiskManager::useLongitudFija(std::string lineaArchivo) {
    std::string atributo;
    std::string registro;
    bool is_string = false;
    int i = 0;

    std::cout << "\tUSE LONGITUD FIJA: " << lineaArchivo << std::endl;

    std::istringstream ss(lineaArchivo);
    
    std::string key;
    std::getline(ss, key, ','); // se supone que es id_post

    // recuperamos el num de bloque con el num de registro
    std::pair<int, int> dataLocation = buscarID(key);

    // Extrae los valores del par
    int NroBloque = dataLocation.first;
    int NroRegistro = dataLocation.second;

    insertionMethod(bPTree, std::atoi(key.c_str()), NroBloque, NroRegistro);

    //Convertir a formato de longitud fija
    for (char c : lineaArchivo) {
        if (c == '"') {
            is_string = !is_string;
        } else if (c == ',') {
            if (is_string) {
                atributo.push_back(c);

            } else {
                if (!atributo.empty()) {
                    registro += atributo;
                }
                int tamañoSobrante = tipoCampo[i] - atributo.length();
                for (int j = 0; j < tamañoSobrante; j++) { // llena como longitud fija
                    registro += " ";
                }
                atributo.clear();
                //registro += ",";
                i++;
            }
        } else {
            atributo.push_back(c);
        }
    }
    registro += atributo;

    //Inserta registros 
    bool bloqueActualizado = false;
    Nodo* bloque = this->freeSpaceInicial;

    do {
        std::string archivoBloque = RUTA_BASE + std::string("Bloques/Bloque") + std::to_string(bloque->numeroBloque) + std::string(".txt");
        std::ifstream archivoReadBloque(archivoBloque, std::ios::app);

        if (!archivoReadBloque) {
            std::cerr << "Error al abrir el archivo: " << archivoBloque << std::endl;
            return;
        }

        std::string linea;
        int ubicacion = -1;
        std::string espaciosLibres = "";

        std::getline(archivoReadBloque, linea);
        std::stringstream ss(linea);
        std::string token;

        std::getline(ss, token, '#'); // Ignorar el identificador
        std::getline(ss, token, '#');

        std::cout << "*** Bloque: " << bloque->numeroBloque << "; Espacio Disponible: " << bloque->espacioLibre << std::endl;

        if (bloque->espacioLibre >= longitudRegistro) {
            std::getline(ss, token, '#');
            if (!token.empty()) {
                espaciosLibres = removerPrimerElemento(token);
                std::stringstream ssToken(token);
                std::getline(ssToken, token, ',');
                ubicacion = std::stoi(token) + 1;
            } else {
                ubicacion = -1;
            }
            std::cout << "\tEspacio libre entre registros: " << ubicacion << std::endl;
        } 
        else {
            archivoReadBloque.close();
            
            actualizarBloque();

            bloqueActualizado = true;
            continue;
        }

        archivoReadBloque.close();

        if (ubicacion == -1) {
            std::ofstream archivoWriteBloque(archivoBloque, std::ios::app);
            if (!archivoWriteBloque) {
                std::cerr << "Error al abrir el archivo para escritura: " << archivoBloque << std::endl;
                return;
            }

            archivoWriteBloque << registro << std::endl;
            archivoWriteBloque.close();
        } else {
            actualizarLineaLongitudFija(archivoBloque, registro, ubicacion);           
        }

        decreaseSpaceofBlock(bloque->numeroBloque);

        CabeceraBloque nuevaCabecera;
        nuevaCabecera.identificador = bloque->numeroBloque;
        nuevaCabecera.espacioDisponible = bloque->espacioLibre;
        nuevaCabecera.espaciosLibres = espaciosLibres;
        reemplazarCabecera(archivoBloque, nuevaCabecera);

        bloqueActualizado = false;
        
        sectorFillLongitudFija(registro, ubicacion, bloque);

    } while (bloqueActualizado);  
}

void DiskManager::sectorFillLongitudFija(const std::string& lineaArchivo, int ubicacion, Nodo*& bloque) {
    bool found = false;
    bool update = true;
    int sectorCont = disco.getCantidadSectoresxBloque();
    auto tupla = bloque->sectores.begin();

    do {
        if (ubicacion != -1) {
            sectorCont = int(((ubicacion * this->longitudRegistro) / disco.getBytesxSector()) + 1); // Calcula el sector al que pertenece
            tupla = bloque->sectores.begin() + sectorCont;
        } else {
            // Se busca un sector libre
            for (auto it = bloque->sectores.begin(); it != bloque->sectores.end(); ++it) {
                bool estado = std::get<4>(*it); // Obtener el estado del sector
                if (!estado) {
                    tupla = it;
                    found = true; // Se encontró un sector con estado false
                    break;
                }
            }
        }

        if (!found && ubicacion == -1) return;

        this->platoAct = std::get<0>(*tupla);
        this->superfAct = std::get<1>(*tupla);
        this->pistaAct = std::get<2>(*tupla);
        this->sectorAct = std::get<3>(*tupla);
        std::cout << "Plato Actual: " << this->platoAct
                    << "; Superficie Actual: " << this->superfAct
                    << "; Pista Actual: " << this->pistaAct
                    << "; Sector Actual: " << this->sectorAct << std::endl;

        std::string archivoSector = RUTA_BASE + std::string("Sectores/") + std::to_string(this->platoAct) + "-" + this->superfAct + "-" + std::to_string(this->pistaAct) + "-" + std::to_string(this->sectorAct) + ".txt";
        std::ifstream archivoReadSector(archivoSector);

        if (!archivoReadSector) {
            std::cerr << "Error al abrir el archivo: " << archivoSector << std::endl;
            return;
        }

        std::string linea;
        int espacioDisponible = 0;
        std::string espaciosLibres;
        std::string ubicacionSector;

        std::getline(archivoReadSector, linea);
        std::stringstream ss(linea);
        std::string token;

        std::getline(ss, token, '#'); // Ignorar el identificador
        std::getline(ss, token, '#');
        espacioDisponible = std::stoi(token);
        std::cout << "Espacio Disponible de Sector: " << espacioDisponible << std::endl;

        std::getline(ss, token, '#');
        if (!token.empty()) {
            espaciosLibres = removerPrimerElemento(token);
            std::stringstream ssToken(token);
            std::getline(ssToken, token, ',');
        }

        std::getline(ss, token, '#');
        ubicacionSector = token;

        archivoReadSector.close();

        if (espacioDisponible >= longitudRegistro) {
            if (ubicacion == -1) {
                std::ofstream archivoWriteSector(archivoSector, std::ios::app);
                if (!archivoWriteSector) {
                    std::cerr << "Error al abrir el archivo para escritura: " << archivoSector << std::endl;
                    return;
                }

                archivoWriteSector << lineaArchivo << std::endl;
                archivoWriteSector.close();
            } else {
                int numRegistroEnSector = (ubicacion % disco.getBytesxSector()) / this->longitudRegistro + 1;
                actualizarLineaLongitudFija(archivoSector, lineaArchivo, numRegistroEnSector);
            }

            espacioDisponible -= longitudRegistro;
            update = false;

        } else {
            for (auto& tupla : bloque->sectores) {
                if (std::get<0>(tupla) == this->platoAct &&
                    std::get<1>(tupla) == this->superfAct &&
                    std::get<2>(tupla) == this->pistaAct &&
                    std::get<3>(tupla) == this->sectorAct) {
                    std::get<4>(tupla) = true; // Cambiar el estado del sector a ocupado
                    break;
                }
            }
            actualizarSector();
        }

        CabeceraSector nuevaCabecera;
        nuevaCabecera.identificador = this->sectorAct;
        nuevaCabecera.espacioDisponible = espacioDisponible;
        nuevaCabecera.espaciosLibres = espaciosLibres;
        nuevaCabecera.ubicacion = ubicacionSector;
        reemplazarCabecera(archivoSector, nuevaCabecera);

    } while (update);
}

void DiskManager::actualizarLineaLongitudFija(int numBloque, const std::string& nuevaLinea, int numRegistro) {
    Nodo* bloque = searchBlockHeapFile(numBloque);

    std::string archivoBloque = RUTA_BASE + std::string("Bloques/Bloque") + std::to_string(bloque->numeroBloque) + std::string(".txt");
    std::ifstream archivoReadBloque(archivoBloque, std::ios::app);
    actualizarLineaLongitudFija(archivoBloque, nuevaLinea, numRegistro);

    int sectorCont = ((numRegistro * this->longitudRegistro) % disco.getBytesxSector()); // Calcula el sector al que pertenece
    const auto& tupla = bloque->sectores.begin() + sectorCont;


    this->platoAct = std::get<0>(*tupla);
    this->superfAct = std::get<1>(*tupla);
    this->pistaAct = std::get<2>(*tupla);
    this->sectorAct = std::get<3>(*tupla);    
    std::cout << "Plato Actual: " << this->platoAct
                << "; Superficie Actual: " << this->superfAct
                << "; Pista Actual: " << this->pistaAct
                << "; Sector Actual: " << this->sectorAct << std::endl;

    std::string archivoSector = RUTA_BASE + std::string("Sectores/") + std::to_string(this->platoAct) + "-" + this->superfAct + "-" + std::to_string(this->pistaAct) + "-" + std::to_string(this->sectorAct) + ".txt";

    int numRegistroEnSector = (numRegistro % disco.getBytesxSector()) / this->longitudRegistro + 1;

    actualizarLineaLongitudFija(archivoSector, nuevaLinea, numRegistroEnSector);
}

void DiskManager::actualizarLineaLongitudFija(const std::string& archivo, const std::string& nuevaLinea, int numRegistro) {
    std::ifstream archivoLeer(archivo);
    std::ofstream archivoTemporal("temp.txt");

    if (!archivoLeer) {
        std::cerr << "Error al abrir el archivo para lectura: " << archivo << std::endl;
        return;
    }
    if (!archivoTemporal) {
        std::cerr << "Error al crear archivo temporal" << std::endl;
        archivoLeer.close();
        return;
    }

    //std::cout << "numRegistrodentro: " << numRegistro << std::endl;

    std::string linea;
    int numeroLinea = 1;

    while (std::getline(archivoLeer, linea)) {
        std::cout << numeroLinea << " -- ";
        std::cout << linea << std::endl;
        if (numeroLinea == numRegistro) {
            archivoTemporal << nuevaLinea << std::endl;
        } else {
            archivoTemporal << linea << std::endl;
        }
        numeroLinea++;
    }

    archivoLeer.close();
    archivoTemporal.close();

    if (std::remove(archivo.c_str()) != 0) {
        std::cerr << "Error al eliminar el archivo original: " << archivo << std::endl;
    }
    if (std::rename("temp.txt", archivo.c_str()) != 0) {
        std::cerr << "Error al renombrar archivo temporal a original: " << archivo << std::endl;
    }
}

void DiskManager::eliminarLineaLongitudFija(int numBloque, int numRegistro) {
    Nodo* bloque = searchBlockHeapFile(numBloque);
    std::string archivoBloque = RUTA_BASE + std::string("Bloques/Bloque") + std::to_string(numBloque) + std::string(".txt");
    std::string lineaEliminada;

    std::ifstream archivoReadBloque(archivoBloque, std::ios::app);
    if (!archivoReadBloque) {
        std::cerr << "Error al abrir el archivo: " << archivoBloque << std::endl;
        return;
    }
    std::string linea;
    std::string espaciosLibres = "";
    std::getline(archivoReadBloque, linea);
    std::stringstream ss(linea);
    std::string token;
    std::getline(ss, token, '#'); // Ignorar el identificador
    std::getline(ss, token, '#');
    std::getline(ss, token, '#');
    espaciosLibres = token;        
    archivoReadBloque.close();

    for(int i = 0; i < this->longitudRegistro; i++) {
        lineaEliminada += " ";
    }

    actualizarLineaLongitudFija(archivoBloque, lineaEliminada, numRegistro);

    int sectorCont = ((numRegistro * this->longitudRegistro) % disco.getBytesxSector()); // Calcula el sector al que pertenece
    const auto& tupla = bloque->sectores.begin() + sectorCont;

    this->platoAct = std::get<0>(*tupla);
    this->superfAct = std::get<1>(*tupla);
    this->pistaAct = std::get<2>(*tupla);
    this->sectorAct = std::get<3>(*tupla);

    std::string archivoSector = RUTA_BASE + std::string("Sectores/") + std::to_string(this->platoAct) + "-" + this->superfAct + "-" + std::to_string(this->pistaAct) + "-" + std::to_string(this->sectorAct) + ".txt";
    std::ifstream archivoReadSector(archivoSector);
    if (!archivoReadSector) {
        std::cerr << "Error al abrir el archivo: " << archivoSector << std::endl;
        return;
    }
    std::string linea2;
    int espacioDisponible = 0;
    std::string espaciosLibresSector;
    std::string ubicacionSector;
    std::getline(archivoReadSector, linea2);
    std::stringstream ss2(linea2);
    std::string token2;
    std::getline(ss2, token2, '#'); // Ignorar el identificador
    std::getline(ss2, token2, '#');
    espacioDisponible = std::stoi(token2);
    std::cout << "Espacio Disponible de Sector: " << espacioDisponible << std::endl;
    std::getline(ss2, token2, '#');
    espaciosLibresSector = token2;
    std::getline(ss2, token2, '#');
    ubicacionSector = token2;
    archivoReadSector.close();

    int numRegistroEnSector = (numRegistro % disco.getBytesxSector()) / this->longitudRegistro + 1;

    actualizarLineaLongitudFija(archivoSector, lineaEliminada, numRegistroEnSector);

    //Se actualiza la cabecera de bloque y sector. También el HEAPFILE

    //HEAPFILE
    bloque->espacioLibre += this->longitudRegistro;

    //Cabecera bloque
    CabeceraBloque nuevaCabeceraBloque;
    nuevaCabeceraBloque.identificador = bloque->numeroBloque;
    nuevaCabeceraBloque.espacioDisponible = bloque->espacioLibre + this->longitudRegistro;
    nuevaCabeceraBloque.espaciosLibres = insertAndSort(espaciosLibres, numRegistro);
    reemplazarCabecera(archivoBloque, nuevaCabeceraBloque);

    //Cabecera sector
    CabeceraSector nuevaCabeceraSector;
    nuevaCabeceraSector.identificador = this->sectorAct;
    nuevaCabeceraSector.espacioDisponible = espacioDisponible + this->longitudRegistro;
    nuevaCabeceraSector.espaciosLibres = insertAndSort(espaciosLibresSector, numRegistroEnSector);
    nuevaCabeceraSector.ubicacion = ubicacionSector;
    reemplazarCabecera(archivoSector, nuevaCabeceraSector);
}

void DiskManager::MenuTree() {
    bool flag = true;
    int option;

    do {
        std::cout << "0: Recuperar arbol\n"
                  << "1: Insertar\n"
                  << "2: Buscar\n"
                  << "3: Imprimir arbol\n"
                  << "4: Borrar\n"
                  << "5: Generar imagen del arbol\n"
                  << "6: Salir" << std::endl;
        std::cout << "\tElija una opcion: ";
        std::cin >> option;

        int key, NroBloque, NroRegistro, idx, tmp;
        std::string text12;
        std::pair<int, int> search_result;

        switch (option) {
            case 0:
                std::cout << "Recuperando el árbol..." << std::endl;
                bPTree->deserialize("bptree.dat");

                if (bPTree->getRoot() == nullptr) {
                    std::cerr << "Error: No se pudo recuperar el árbol." << std::endl;
                } else {
                    std::cout << "Árbol recuperado exitosamente." << std::endl;
                }
                break;

            case 1:
                std::cout << "\nIndica Postid, NroBloque, NroRegistro, texto: ";
                std::cin >> key >> NroBloque >> NroRegistro;
                std::getline(std::cin >> std::ws, text12); // Leer el texto con espacios
                insertionMethod(bPTree, key, NroBloque, NroRegistro);
                // ESTA FUNCION ES PARA LLENAR EL ARBOL, SOLO RECIBE
                // ID (KEY) NUM_BLOQUE Y NUM_REGISTRO
                break;

            case 2:
                std::cout << "¿Cuál es el key? ";
                std::cin >> idx;

                search_result = searchMethod(bPTree, idx);

                if (search_result.first != -1 && search_result.second != -1) {
                    std::cout << "Encontrado en bloque " << search_result.first << ", registro " << search_result.second << std::endl;
                    showBlockandRegisterContent(search_result.first, search_result.second);
                } else {
                    std::cout << "Clave no encontrada" << std::endl;
                }
                break;

            case 3:
                printMethod(bPTree);
                break;

            case 4:
                std::cout << "Clave a borrar: ";
                std::cin >> tmp;
                deleteMethod(bPTree, tmp);
                break;

            case 5:
                bPTree->toDot("bptree.dot");
                std::cout << "El archivo DOT se ha generado como 'bptree.dot'. Usa Graphviz para visualizarlo." << std::endl;
                break;

            case 6:
                flag = false;
                break;

            default:
                std::cerr << "Opcion no valida. Intentalo de nuevo." << std::endl;
                break;
        }
    } while (flag);
}


// ============================================= LONGITUD VARIABLE  =====================================================



// ======================================================================================================================


DiskManager::~DiskManager() {
    //delete[] freeSpaceMap;

    std::string pathBloque = RUTA_BASE + std::string("/Bloques");
    std::string pathSector = RUTA_BASE + std::string("/Sectores");

    /*try {
        // Verificar si la carpeta existe
        if (fs::exists(pathBloque) && fs::is_directory(pathBloque) && fs::exists(pathSector) && fs::is_directory(pathSector) ) {
            // Eliminar la carpeta y su contenido
            fs::remove_all(pathBloque);
            fs::remove_all(pathSector);
            std::cout << "Carpetas eliminada correctamente.\n";
        } else {
            std::cerr << "Las carpetas no existen o no es una carpeta válida.\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error al eliminar la carpeta: " << e.what() << '\n';
    }*/
}


// ================================= OPERACIONES CON EL STRUCT =========================================

void eliminarLinea(const std::string& archivoEntrada, const std::string& archivoSalida, int numLineaEliminar) {
    std::ifstream archivoLectura(archivoEntrada);
    if (!archivoLectura) {
        std::cerr << "No se pudo abrir el archivo para leer: " << archivoEntrada << std::endl;
        return;
    }

    std::ofstream archivoEscritura(archivoSalida);
    if (!archivoEscritura) {
        std::cerr << "No se pudo abrir el archivo para escribir: " << archivoSalida << std::endl;
        return;
    }

    std::string linea;
    int numeroLinea = 0;
    while (std::getline(archivoLectura, linea)) {
        numeroLinea++;
        if (numeroLinea != numLineaEliminar) {
            archivoEscritura << linea << std::endl;
        }
    }

    archivoLectura.close();
    archivoEscritura.close();

    // Renombrar el archivo de salida al nombre del archivo original
    std::rename(archivoSalida.c_str(), archivoEntrada.c_str());
}

template <typename Cabecera>
void reemplazarCabecera(const std::string& archivo, const Cabecera& nuevaCabecera) {
    std::string archivoTemporal = "temporal.txt";
    std::ifstream archivoRead(archivo);
    if (!archivoRead) {
        std::cerr << "No se pudo abrir el archivo para leer: " << archivo << std::endl;
        return;
    }

    std::ofstream archivoTempWrite(archivoTemporal);
    if (!archivoTempWrite) {
        std::cerr << "No se pudo abrir el archivo temporal para escribir: " << archivoTemporal << std::endl;
        return;
    }

    std::string linea;
    if (std::getline(archivoRead, linea)) { } // Cabecera original leída y descartada

    // Leer el contenido original del archivo y escribirlo al archivo temporal
    while (std::getline(archivoRead, linea)) {
        archivoTempWrite << linea << std::endl;
    }

    archivoRead.close();
    archivoTempWrite.close();

    // Abrir el archivo original en modo de escritura para reemplazar la cabecera
    std::ofstream archivoWrite(archivo);
    if (!archivoWrite) {
        std::cerr << "No se pudo abrir el archivo para escribir: " << archivo << std::endl;
        return;
    }

    // Escribir la nueva cabecera en el archivo original
    archivoWrite << nuevaCabecera.toString() << std::endl;

    // Leer el contenido del archivo temporal y escribirlo al archivo original
    std::ifstream archivoTempRead(archivoTemporal);
    if (!archivoTempRead) {
        std::cerr << "No se pudo abrir el archivo temporal para leer: " << archivoTemporal << std::endl;
        return;
    }

    while (std::getline(archivoTempRead, linea)) {
        archivoWrite << linea << std::endl;
    }

    archivoTempRead.close();
    archivoWrite.close();

    std::remove(archivoTemporal.c_str());
}

std::vector<int> splitStringToVector(const std::string& str, char delimiter) {
    std::vector<int> elements;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        elements.push_back(std::stoi(item));
    }
    return elements;
}

std::string joinVectorToString(const std::vector<int>& vec, char delimiter) {
    std::stringstream ss;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0) {
            ss << delimiter;
        }
        ss << vec[i];
    }
    return ss.str();
}

std::string insertAndSort(const std::string& str, int num) {
    std::vector<int> elements = splitStringToVector(str, ',');
    elements.push_back(num);
    std::sort(elements.begin(), elements.end());
    return joinVectorToString(elements, ',');
}

// ============================================= ADICIONALES =========================================
std::string removerPrimerElemento(const std::string& cadena) {
    std::cout << cadena << std::endl;
    size_t pos = cadena.find(',');
    if (pos == std::string::npos) {
        return "";
    }
    return cadena.substr(pos + 1);
}

