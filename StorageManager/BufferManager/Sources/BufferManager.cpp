#include <iostream>
#include <cassert>
#include "BufferManager.h"
#include <cstdlib>

BufferManager::BufferManager(int BufferSize, int capacidad) : bufferPool(BufferSize, capacidad), pageTable() {}

void BufferManager::requestPage(int pageID, char operation, std::vector<std::string> Data, DiskManager* disco) {
    if (pageTable.GetSize() == bufferPool.GetSize() && !pageTable.IsPageMapped(pageID)) {
        if (bufferPool.AllFramesInUse()) {
            char respuesta;
            std::cout << "El buffer pool esta lleno y todos los frames estan en uso. Desea reiniciar el buffer pool? (S/N): ";
            std::cin >> respuesta;
            if (respuesta == 'S' || respuesta == 's') {
                bufferPool.ResetBufferPool();
                pageTable.ResetPageTable();
                std::cout << "Buffer pool y tabla de paginas reiniciados.\n";
            } else {
                std::cout << "Operacion cancelada.\n";
                return;
            }
        } else {
            char strategy;
            std::cout << "El buffer pool está lleno. Por favor, seleccione la estrategia de reemplazo ('L' para LRU, 'C' para CLOCK): ";
            std::cin >> strategy;
            if (strategy != 'L' && strategy != 'C') {
                std::cout << "Estrategia de reemplazo no válida. Utilizando LRU por defecto." << std::endl;
                strategy = 'L';
            }
            int aux;
            if (strategy == 'L') {
                aux = bufferPool.LRU();
                std::cout << aux << std::endl;
                for (const auto& entry : pageTable.pageMap) {
                    if (entry.second == aux)
                        aux = entry.first;
                }
                std::cout << aux << std::endl;
                releasePage(aux);
            } else {
                aux = bufferPool.CLOCK();
                for (const auto& entry : pageTable.pageMap) {
                    if (entry.second == aux)
                        aux = entry.first;
                }
                releasePage(aux);
            }
        }
    }

    if (operation == 'L' || operation =='l') {
        if (!pageTable.IsPageMapped(pageID)) {
            int frameID = bufferPool.FindUnpinnedFrame();
            pageTable.MapPageToFrame(pageID, frameID);
            bufferPool.IncremetFrame(frameID);
            
            
            for (const auto& entry : pageTable.pageMap) {
                if (pageID != entry.first){
                    Frame* frame = bufferPool.GetFrame(entry.second);
                    frame->increment();
                    frame->refOn();
                }
                else {
                    Frame* frame = bufferPool.GetFrame(entry.second);
                    frame->used();
                    frame->refOn();
                    bufferPool.UpdateIndex();
                    frame->SetRequest(operation);
                    bufferPool.SetData(Data, entry.second);
                }
            }
        } else {
            int frameID = pageTable.pageMap[pageID];
            bufferPool.IncremetFrame(frameID);
            
            for (const auto& entry : pageTable.pageMap) {
                if (pageID != entry.first){
                    Frame* frame = bufferPool.GetFrame(entry.second);
                    frame->increment();
                    frame->refOn();
                }
                else {
                    Frame* frame = bufferPool.GetFrame(entry.second);
                    frame->used();
                    frame->refOn();
                    bufferPool.UpdateIndex();
                    frame->SetRequest(operation);
                    bufferPool.SetData(Data, entry.second);
                }
            }
        }
    }
    else if (operation == 'W' || operation =='w'){
        if (!pageTable.IsPageMapped(pageID)) {
            int frameID = bufferPool.FindUnpinnedFrame();
            pageTable.MapPageToFrame(pageID, frameID);
            bufferPool.IncremetFrame(frameID);
            bufferPool.DirtyFrame(frameID);   
            for (const auto& entry : pageTable.pageMap) {
                if (pageID != entry.first){
                    Frame* frame = bufferPool.GetFrame(entry.second);
                    frame->increment();
                    frame->refOn();
                }
                else {
                    Frame* frame = bufferPool.GetFrame(entry.second);
                    frame->used();
                    frame->refOn();
                    bufferPool.UpdateIndex();
                    frame->SetRequest(operation);
                    bufferPool.SetData(Data, entry.second);
                }
            }
        } 
        else {
            // La página ya está mapeada, actualizar el dato en el buffer pool
            int frameID = pageTable.pageMap[pageID];
            bufferPool.IncremetFrame(frameID);
            bufferPool.DirtyFrame(frameID);
            for (const auto& entry : pageTable.pageMap) {
                if (pageID != entry.first){
                    Frame* frame = bufferPool.GetFrame(entry.second);
                    frame->increment();
                    frame->refOn();
                }
                else {
                    Frame* frame = bufferPool.GetFrame(entry.second);
                    frame->used();
                    frame->refOn();
                    bufferPool.UpdateIndex();
                    frame->SetRequest(operation);
                    bufferPool.SetData(Data, entry.second);
                }
            }
        }
            ModRegister(pageID, disco);
    }
}

void BufferManager::releasePage(int pageID) {
    int frameID = pageTable.pageMap[pageID];
    Frame* frame = bufferPool.GetFrame(frameID);

    if(frame->GetRequestFront() == 'W' || frame->GetRequestFront() == 'w'){
        if (frame->GetDirty() ) {
            char c;
            std::cout << "Desea guardar la pagina S/N" << std::endl;
            std::cin>>c; 
            if(c == 'S' || c == 's') {
                std::cout << "pagina guardada" << std::endl;
            }
            else {
                std::cout << "pagina no guardada" << std::endl;
            }
        }
        frame->ReleaseDirty();
    }

    if (frame->GetPinCount() == 0) {

        Frame* frame = bufferPool.GetFrame(frameID);
        frame->reset();
        bufferPool.ReleaseFrame(frameID);
        pageTable.UnmapPage(pageID);
        bufferPool.UpdateIndex(); // Opcionalmente actualizar el índice para la próxima política CLOCK

        std::cout << "La pagina " << pageID << " ha sido liberada del marco " << frameID << "." << std::endl;
    }
    else{
        Frame* frame = bufferPool.GetFrame(frameID);
        frame->DecrementCount();
        frame->ReleaseRequest();
    }
}

void BufferManager::printPageTable() {
    std::cout << "# Frame ID\t- Page ID\t- Dirty Bit\t- Pin Count\t- Pinned\t- Last Used\t- Ref. Bit\n";
    for (const auto& entry : pageTable.pageMap) {
        Frame* frame = bufferPool.GetFrame(entry.second);
        std::cout << "# " << entry.second << "\t\t- " << entry.first << "\t\t- "
                  << (frame->GetDirty() ? "1" : "0") << "\t\t- "
                  << frame->GetPinCount() << "\t\t- " << frame->GetIsPinned() << "\t\t- " << frame->GetLastUsed() << "\t\t- " << frame->GetRefBit() << "\t\t\n";
    }
    for (const auto& entry : pageTable.pageMap) {
        Frame* frame = bufferPool.GetFrame(entry.second);
        std::cout << "Los requerimientos del frame " << entry.second << " son:" << std::endl;
        frame->PrintRequest();
    }
}
void BufferManager::PinearPagina(int pageID) {
    auto it = pageTable.pageMap.find(pageID);
    if (it != pageTable.pageMap.end()) {
        int frameID = it->second;
        Frame* frame = bufferPool.GetFrame(frameID);
        frame->pin();
    } else {
        std::cout << "La pagina no se encuentra" << std::endl;
    }
}

void BufferManager::PrintRequest() {
    for (const auto& entry : pageTable.pageMap) {
        Frame* frame = bufferPool.GetFrame(entry.second);
        std::cout << "Los requerimientos del frame " << entry.second << " son:" << std::endl;
        frame->PrintRequest();
    }
}

void BufferManager::ViewPagina(int pageID) {
    system("cls");  // Limpia la pantalla
    auto it = pageTable.pageMap.find(pageID);
    if (it != pageTable.pageMap.end()) {
        int frameID = it->second;
        Frame* frame = bufferPool.GetFrame(frameID);
        std::cout << "El contenido de la pagina " << pageID << " es:" << std::endl; 
        frame->ViewData();   
    } else {
        std::cout << "La pagina no se encuentra" << std::endl;
    }  
    system("pause");
    system("cls");  // Limpia la pantalla
}

void BufferManager::ModRegister(int pageID, DiskManager* disco) {
    auto it = pageTable.pageMap.find(pageID);
    if (it != pageTable.pageMap.end()) {
        int frameID = it->second;
        Frame* frame = bufferPool.GetFrame(frameID);

        bool keepRunning = true;
        while (keepRunning) {
            std::cout << "Seleccione una opcion:" << std::endl;
            std::cout << "1. Actualizar registro"<< std::endl;
            std::cout << "2. Insertar nuevo registro" << std::endl;
            std::cout << "3. Eliminar registro" << std::endl;
            std::cout << "4. Salir"<< std::endl;

            int option;
            std::cin >> option;
            switch(option) {
                case 1: {    // Actualizar
                    int NroRegister;
                    ViewPagina(pageID);
                    std::cout << "Que registro deseas Modificar:" << std::endl;

                    std::cin >> NroRegister;


                    std::vector<int> index = frame->GetIndex(NroRegister);
                    std::vector<std::string> registeer = frame->GetRegister(NroRegister);
                    std::cout << std::endl;
                    
                    frame->ViewRegister(NroRegister);
                    std::cout << "____________________________________" << std::endl;
                    for(int i = 1; i < registeer.size(); i++) {
                        std::cout << "Columa " << i << ": " << registeer[i] << std::endl; 
                    } 

                    int col;
                    std::cout << "Que columna desea Modificar:" << std::endl; 
                    std::cin >> col;
                    if (col < index.size()) {
                        std::cout << registeer[col] << std::endl;
                        std::cout << "_____________________________________________" << std::endl;

                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                        std::string aux;
                        std::getline(std::cin, aux);
                        
                        if(aux.size() <= index[col]) 
                            aux = aux.substr(0, index[col]);

                        for(int i = aux.size(); i < index[col]; i ++) 
                            aux += " ";

                        registeer[col] = aux;
                        frame->SetRegister(NroRegister, registeer);

                    } else {
                        std::cout << "Columna excedida" << std::endl;
                    }
                    break;
                }
                case 2: { // Insertar

                    std::vector<int> index = frame->GetIndex(1);
                    std::vector<std::string> newRegister(index.size());

                    for(int i = 1; i < index.size(); i++) {
                        std::cout << "Ingrese el valor para la columna " << i << ": ";
                        std::string aux;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, aux);

                        if(aux.size() <= index[i]) 
                            aux = aux.substr(0, index[i]);

                        for(int j = aux.size(); j < index[i]; j ++) 
                            aux += " ";

                        newRegister[i] = aux;
                    }

                    frame->AddRegister(newRegister);
                    std::cout << "Registro insertado correctamente." << std::endl;
                    break;
                }
                case 3: { // Eliminar

                    int claveBusqueda;
                    ViewPagina(pageID);

                    int clave_indice;
                    /*
                        clave_indice = indice por el cual se va a borrar
                            post_id = 0
                            user_id = 1  
                            fecha = 2   
                            text = 3
                    */
                    clave_indice = 0;

                    std::cout << "Que registro deseas Eliminar:\n";
                    std::cin >> claveBusqueda;

                    // aux va a ser llenado con los registros que estan en la pagina
                    
                    
                    // i va a ser la linea que va a ser eliminada
                    // frame->GetRegister(i) te devuelve el registro en forma de vector

                    for(int i = 1; i <= frame->GetDataSize(); i++){
                        std::vector<std::string> index = frame->GetRegister(i);
                        std::cout <<  index[clave_indice] <<std::endl;
                        if(std::atoi(index[clave_indice].c_str()) == claveBusqueda){
                            std::cout << i <<std::endl;
                            frame->RemoveRegister(i);

                            // esto de borra de inmediato al disco luego de borrar en frame
                            // faltaria corregir esa logica
                            // esta funcion deberia estar al momento de reemplazar pagina
                            // o cuando se guarda a disco por voluntad
                            //disco->eliminar(pageID,i+1,claveBusqueda);
                            break;
                        }
                    }
                    std::cout << "Registro eliminado correctamente." << std::endl;
                    
                    //keepRunning = false; // Exit after deletion as the NroRegister may not be valid anymore
                    break;
                }
                case 4: {
                    keepRunning = false;
                    break;
                }
                default: {
                    std::cout << "Opcion invalida. Intente nuevamente." << std::endl;
                    break;
                }
            }
        }
    } else {
        std::cout << "La pagina no se encuentra" << std::endl;
    } 
}

