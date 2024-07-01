#include "diskManager.h"

void eliminarLinea(const std::string& archivoEntrada, const std::string& archivoSalida, int numLineaEliminar);
void reemplazarCabecera(const std::string& archivo, const CabeceraSector& nuevaCabecera);

std::string removerPrimerElemento(const std::string& cadena);


DiskManager::DiskManager() : disco(0, 0, 0, 0, 0) {}

DiskManager::DiskManager(bool tipo, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque) : disco(nroPlatos, nroPistas, nroSectores, bytesxSector, sectoresxBloque), tipoLongitud(tipo), tipoCampo(nullptr), freeSpaceInicial(nullptr), freeSpaceFinal(nullptr), fullSpaceInicial(nullptr), fullSpaceFinal(nullptr) {
    this->platoAct = 1;
    this->superfAct = 'A';
    this->pistaAct = 1;
    this->sectorAct = 1;
    this->bloqueAct = 1;
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
    if (this->sectorAct > disco.getCantidadSectores()) {
            this->sectorAct = 1;
            this->pistaAct++;
            if (this->pistaAct > disco.getCantidadPistas()) {
                this->pistaAct = 1;
                this->superfAct = (this->superfAct == 'A') ? 'B' : 'A';
                if (this->superfAct == 'A') {
                    this->platoAct++;
                    if (this->platoAct > disco.getCantidadPlatos())
                        this->platoAct = 1;
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
        archivo << this->sectorAct << "#" << disco.getBytesxSector() << "#0#" << std::endl;
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

    for (int bloque = 1; bloque <= disco.getCantidadBloques(); bloque++)
    {
        std::string archivoBloque = carpetaBloques + "/Bloque" + std::to_string(bloque) + ".txt";
        std::ofstream archivo(archivoBloque);
        if (!archivo.is_open()) {
            std::cerr << "Error al crear el archivo " << archivoBloque << '\n';
        }
        
        std::string contenido = std::to_string(bloque) + "#" + std::to_string(disco.getTamañoBloque()) + "##";
        archivo << contenido << std::endl;

        std::unordered_map<int, std::tuple<int, char, int, bool>> sectores;
        for (int sector = 1; sector <= disco.getCantidadSectoresxBloque(); sector++) {
            sectores[this->sectorAct] = std::make_tuple(this->platoAct, this->superfAct, this->pistaAct, false);

            this->sectorAct++;

            validarUbicacionActual();
        }
        insertBlocktoFreeHeapFile(bloque, sizeBlock, sectores);

        archivo.close();
    }

    this->platoAct = 1;
    this->superfAct = 'A';
    this->pistaAct = 1;
    this->sectorAct = 1;

    std::cout << "La estructura del disco ha sido creada exitosamente." << std::endl;
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

std::vector<std::string> DiskManager::readBlockToVector(int numBloque) {
    std::vector<std::string> registros;
    std::string carpetaBloque = RUTA_BASE + std::string("Bloques/");
    std::string archivoBloque = carpetaBloque + std::string("Bloque") + std::to_string(numBloque) + ".txt";
    std::ifstream file(archivoBloque);

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << archivoBloque << std::endl;
        return registros;
    }

    std::string line;
    std::getline(file, line); // Ignorar la cabecera
    while (std::getline(file, line)) {
        registros.push_back(line);
    }

    file.close();
    return registros;
}

// ============================================ HEAP FILE ===================================================

void DiskManager::insertBlocktoFreeHeapFile(int numBloque, int espLibre, const std::unordered_map<int, std::tuple<int, char, int, bool>>& sectores) {
    Nodo* nuevoNodo = new Nodo(numBloque, espLibre);
    nuevoNodo->sectores = sectores;

    if (!this->freeSpaceInicial) {
        this->freeSpaceInicial = this->freeSpaceFinal = nuevoNodo;
        this->bloqueAct = this->freeSpaceInicial->numeroBloque;
        auto it = this->freeSpaceInicial->sectores.begin();
        this->sectorAct = it->first;
        if (it != this->freeSpaceInicial->sectores.end()) {
            this->platoAct = std::get<0>(it->second);
            this->superfAct = std::get<1>(it->second);
            this->pistaAct = std::get<2>(it->second);
        }
    } else {
        this->freeSpaceFinal->next = nuevoNodo;
        nuevoNodo->prev = this->freeSpaceFinal;
        this->freeSpaceFinal = nuevoNodo;
    }
}

void DiskManager::insertBlocktoFullHeapFile(int numBloque, int espLibre, const std::unordered_map<int, std::tuple<int, char, int, bool>>& sectores) {
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
    Nodo* actual = this->fullSpaceInicial;
    while (actual) {
        printBlockInformation(actual);
        actual = actual->next;
    }
    if(!this->fullSpaceInicial)
        std::cout << "La lista de Bloques llenos esta vacia" << std::endl;
}

void DiskManager::printBlockInformation(Nodo* nodo) {
    std::cout << "\tBloque: " << nodo->numeroBloque
                << ", Espacio Libre: " << nodo->espacioLibre
                << ", Sectores: ";
    for (const auto& [idSector, tupla] : nodo->sectores) {
        auto [plato, superficie, pista, estado] = tupla;
        std::cout << "(" << idSector << ", " << plato << ", " << superficie << ", " << pista << ", " << estado << ") ";
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

    bloque->espacioLibre = bloque->espacioLibre - longitudRegistro;

    std::cout << " -- Bloque " << bloque->numeroBloque << " con espacio libre reducido a " << bloque->espacioLibre << std::endl;
    
    if(bloque->espacioLibre < longitudRegistro) {
        moveBlockFreeToFull(bloque);
        this->bloqueAct = this->freeSpaceInicial->numeroBloque;
        auto it = this->freeSpaceInicial->sectores.begin();
        this->sectorAct = it->first;
        if (it != this->freeSpaceInicial->sectores.end()) {
            this->platoAct = std::get<0>(it->second);
            this->superfAct = std::get<1>(it->second);
            this->pistaAct = std::get<2>(it->second);
        }
    }
}

void DiskManager::increaseSpaceofBlock(int numBloque) {
    Nodo* bloque = searchBlockHeapFile(numBloque);

    if (!bloque) {
        std::cout << "Bloque con numero " << numBloque << " no encontrado." << std::endl;
        return;
    }

    bloque->espacioLibre = bloque->espacioLibre + longitudRegistro;
    std::cout << "++ Bloque " << bloque->numeroBloque << " con espacio libre incrementado a " << bloque->espacioLibre << std::endl;
    
    if(searchFullSpace(bloque->numeroBloque) && bloque->espacioLibre >= longitudRegistro){
        moveBlockFullToFree(bloque);
        this->bloqueAct = this->freeSpaceInicial->numeroBloque;
        auto it = this->freeSpaceInicial->sectores.begin();
        this->sectorAct = it->first;
        if (it != this->freeSpaceInicial->sectores.end()) {
            this->platoAct = std::get<0>(it->second);
            this->superfAct = std::get<1>(it->second);
            this->pistaAct = std::get<2>(it->second);
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

void DiskManager::saveHeapFile() {
    std::ofstream archivo(RUTA_BASE + std::string("heapFile.txt"));

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    saveInformationInFile(this->freeSpaceInicial, archivo);
    saveInformationInFile(this->fullSpaceInicial, archivo);

    archivo.close();
}

void DiskManager::saveInformationInFile(Nodo* head, std::ofstream& archivo) {
    Nodo* actual = head;
    while (actual) {
        archivo << actual->numeroBloque << " " << actual->espacioLibre << " ";

        for (const auto& [idSector, tupla] : actual->sectores) {
            auto [plato, superficie, pista, estado] = tupla;
            archivo << idSector << ":" << plato << ":" << superficie << ":" << pista << ",";
        }

        archivo << std::endl;
        actual = actual->next;
    }
}

void DiskManager::recoverInformationFromHeapFile() {
    std::ifstream archivo(RUTA_BASE + std::string("heapFile.txt"));

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    vaciarHeapFile(); // Eliminar nodos existentes antes de recuperar la información

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::istringstream iss(linea);
        char lista;
        int numBloque, espLibre;
        char separador;
        std::unordered_map<int, std::tuple<int, char, int, bool>> sectores;

        iss >> lista >> numBloque >> espLibre;

        while (iss >> numBloque >> separador) {
            int plato, superficie, pista;
            char separador2;
            bool estado;
            iss >> plato >> separador2 >> superficie >> separador2 >> pista >> separador2 >> estado >> separador;
            sectores[numBloque] = std::make_tuple(plato, superficie, pista, estado);
        }

        if (lista == 'F') {
            insertBlocktoFreeHeapFile(numBloque, espLibre, sectores); // Inserta en FreeSpace
        } else if (lista == 'S') {
            insertBlocktoFullHeapFile(numBloque, espLibre, sectores); // Inserta en FullSpace
        }
    }

    archivo.close();
}


// ============================================= LONGITUD FIJA ==========================================================

/*
    * Calcula el tamaño de un registro por esquema (LONGITUD FIJA)
*/
void DiskManager::setSizeScheme(const std::string& esquema) {
    std::stringstream ss(esquema);
    std::string campo;
    std::string tipo;

    int size = 0;
    int i = 0;

    std::stringstream ssCount(esquema);
    std::string temp;
    int count = 0;
    std::getline(ssCount, temp, '#');

    while (std::getline(ssCount, temp, '#')) {
        count++;
    }
    count /= 2; 

    tipoCampo = new int[count];
    
    std::getline(ss, temp, '#');
    while (std::getline(ss, campo, '#') && std::getline(ss, tipo, '#')) {
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

    std::cout << "Tamanio del registro: " << size << std::endl;
    setLongitudRegistro(size);
    
    for (int j = 0; j < i; ++j) {
        std::cout << "Type size at index " << j << ": " << tipoCampo[j] << " bytes\n";
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

                for (int j = 0; j < tamañoSobrante; j++) {
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

    bool sectorActualizado = false;
    Nodo* bloque = this->freeSpaceInicial;

    do {
        std::string archivoBloque = RUTA_BASE + std::string("/Bloques/Bloque") + std::to_string(bloque->numeroBloque) + std::string(".txt");
        std::ifstream archivoReadBloque(archivoBloque, std::ios::app);

        if (!archivoReadBloque) {
            std::cerr << "Error al abrir el archivo: " << archivoBloque << std::endl;
            return;
        }

        std::string linea;
        int ubicacion = 0;
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
                ubicacion = 0;
            }
            std::cout << "\tEspacio libre entre registros: " << ubicacion << std::endl;
        } 
        /*else {
            archivoReadBloque.close();
            this->bloqueAct++;
            sectorActualizado = true;
            continue;
        }*/

        archivoReadBloque.close();

        if (ubicacion == 0) {
            std::ofstream archivoWriteSector(archivoBloque, std::ios::app);
            if (!archivoWriteSector) {
                std::cerr << "Error al abrir el archivo para escritura: " << archivoBloque << std::endl;
                return;
            }

            archivoWriteSector << registro << std::endl;
            archivoWriteSector.close();
        } else {
            actualizarLineaLongitudFija(archivoBloque, registro, ubicacion);           
        }

        decreaseSpaceofBlock(bloque->numeroBloque);

        CabeceraSector nuevaCabecera;
        nuevaCabecera.identificador = bloque->numeroBloque;
        nuevaCabecera.espacioDisponible = bloque->espacioLibre;
        nuevaCabecera.espaciosLibres = espaciosLibres;
        reemplazarCabecera(archivoBloque, nuevaCabecera);

        sectorActualizado = false;
        
        sectorFillLongitudFija(registro, ubicacion, bloque);

    } while (sectorActualizado);  
}

void DiskManager::sectorFillLongitudFija(const std::string& lineaArchivo, int ubicacion, Nodo*& bloque) {
    bool estado = true;

    for (const auto& par : bloque->sectores) {
        const auto& [idSector, tupla] = par;
        estado = std::get<3>(tupla);
        if (!estado) {
            this->platoAct = std::get<0>(tupla);
            this->superfAct = std::get<1>(tupla);
            this->pistaAct = std::get<2>(tupla);
            this->sectorAct = idSector;
            std::cout << "Plato Actual: " << this->platoAct << "; Superficie Actual: " << this->superfAct << "; Pista Actual: " << this->pistaAct << "; Sector Actual: " << this->sectorAct << std::endl;
            break; // Se encontró un sector con estado false, se imprime y se sale de la función
        }
        
    }
    if(estado) return;
    
    std::string archivoSector = RUTA_BASE + std::string("Sectores/") + std::to_string(this->platoAct) + "-" + this->superfAct + "-" + std::to_string(this->pistaAct) + "-" + std::to_string(this->sectorAct) + ".txt";
    std::ifstream archivoReadSector(archivoSector);

    if (!archivoReadSector) {
        std::cerr << "Error al abrir el archivo: " << archivoSector << std::endl;
        return;
    }

    std::string linea;
    int espacioDisponible = 0;
    int numRegistros = 0;
    std::string espaciosLibres;

    std::getline(archivoReadSector, linea);
    std::stringstream ss(linea);
    std::string token;


    std::getline(ss, token, '#'); // Ignorar el identificador
    std::getline(ss, token, '#');
    espacioDisponible = std::stoi(token);
    std::getline(ss, token, '#');
    numRegistros = std::stoi(token);
    std::cout << "Espacio Disponible de Sector: " << espacioDisponible << std::endl;

    archivoReadSector.close();

    if (espacioDisponible >= longitudRegistro) {

        if (!ubicacion) {
            std::ofstream archivoWriteSector(archivoSector, std::ios::app);
            if (!archivoWriteSector) {
                std::cerr << "Error al abrir el archivo para escritura: " << archivoSector << std::endl;
                return;
            }

            archivoWriteSector << lineaArchivo << std::endl;
            archivoWriteSector.close();
        } else {
            actualizarLineaLongitudFija(archivoSector, lineaArchivo, ubicacion);           
        }

        numRegistros++;
        espacioDisponible -= longitudRegistro;

        std::cout << "SECTOR -- espacioDisponibleDESPUÉS DE RESTA: " << espacioDisponible << std::endl;

        CabeceraSector nuevaCabecera;
        nuevaCabecera.identificador = this->sectorAct;
        nuevaCabecera.espacioDisponible = espacioDisponible;
        nuevaCabecera.numRegistros = numRegistros;
        reemplazarCabecera(archivoSector, nuevaCabecera);
    }

    if(espacioDisponible < longitudRegistro) {
        actualizarSector();
    }
}

void DiskManager::actualizarLineaLongitudFija(const std::string& archivoSector, const std::string& nuevaLinea, int numRegistro) {
    std::ifstream archivoLeer(archivoSector);
    std::ofstream archivoTemporal("temp.txt");

    if (!archivoLeer) {
        std::cerr << "Error al abrir el archivo para lectura: " << archivoSector << std::endl;
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

    if (std::remove(archivoSector.c_str()) != 0) {
        std::cerr << "Error al eliminar el archivo original: " << archivoSector << std::endl;
    }
    if (std::rename("temp.txt", archivoSector.c_str()) != 0) {
        std::cerr << "Error al renombrar archivo temporal a original: " << archivoSector << std::endl;
    }
}

void DiskManager::actualizarSector() {
    this->sectorAct++;
    std::cout << "Actualizando al siguiente sector: " << sectorAct << std::endl;

    validarUbicacionActual();
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

void reemplazarCabecera(const std::string& archivo, const CabeceraSector& nuevaCabecera) {
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

// ============================================= ADICIONALES =========================================
std::string removerPrimerElemento(const std::string& cadena) {
    std::cout << cadena << std::endl;
    size_t pos = cadena.find(',');
    if (pos == std::string::npos) {
        return "";
    }
    return cadena.substr(pos + 1);
}