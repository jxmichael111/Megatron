#include <iostream>
#include <cassert>
#include "BufferManager.h"
#include <conio.h>


BufferManager::BufferManager(int BufferSize, int capacidad) : BufferPool(BufferSize, capacidad), PageTable() {}

void BufferManager::requestPage(int pageID, char operation, std::vector<std::string> Data) {
    if (PageTable.GetSize() == BufferPool.GetSize() && !PageTable.IsPageMapped(pageID)) {
        if (BufferPool.AllFramesInUse()) {
            char respuesta;
            std::cout << "El buffer pool esta lleno y todos los frames estan en uso. Desea reiniciar el buffer pool? (S/N): ";
            std::cin >> respuesta;
            if (respuesta == 'S' || respuesta == 's') {
                BufferPool.ResetBufferPool();
                PageTable.ResetPageTable();
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
                aux = BufferPool.LRU();
                std::cout << aux << std::endl;
                for (const auto& entry : PageTable.pageMap) {
                    if (entry.second == aux)
                        aux = entry.first;
                }
                std::cout << aux << std::endl;
                releasePage(aux);
            } else {
                aux = BufferPool.CLOCK();
                for (const auto& entry : PageTable.pageMap) {
                    if (entry.second == aux)
                        aux = entry.first;
                }
                releasePage(aux);
            }
        }
    }

    if (operation == 'L' || operation =='l') {
        if (!PageTable.IsPageMapped(pageID)) {
            int frameID = BufferPool.FindUnpinnedFrame();
            PageTable.MapPageToFrame(pageID, frameID);
            BufferPool.IncremetFrame(frameID);
            
            
            for (const auto& entry : PageTable.pageMap) {
                if (pageID != entry.first){
                    Frame* frame = BufferPool.GetFrame(entry.second);
                    frame->increment();
                    frame->refOn();
                }
                else {
                    Frame* frame = BufferPool.GetFrame(entry.second);
                    frame->used();
                    frame->refOn();
                    BufferPool.UpdateIndex();
                    frame->SetRequest(operation);
                    BufferPool.SetData(Data, entry.second);
                }
            }
        } else {
            int frameID = PageTable.pageMap[pageID];
            BufferPool.IncremetFrame(frameID);
            
            for (const auto& entry : PageTable.pageMap) {
                if (pageID != entry.first){
                    Frame* frame = BufferPool.GetFrame(entry.second);
                    frame->increment();
                    frame->refOn();
                }
                else {
                    Frame* frame = BufferPool.GetFrame(entry.second);
                    frame->used();
                    frame->refOn();
                    BufferPool.UpdateIndex();
                    frame->SetRequest(operation);
                    BufferPool.SetData(Data, entry.second);
                }
            }
        }
    }
    else if (operation == 'W' || operation =='w'){
        if (!PageTable.IsPageMapped(pageID)) {
            int frameID = BufferPool.FindUnpinnedFrame();
            PageTable.MapPageToFrame(pageID, frameID);
            BufferPool.IncremetFrame(frameID);
            BufferPool.DirtyFrame(frameID);   
            for (const auto& entry : PageTable.pageMap) {
                if (pageID != entry.first){
                    Frame* frame = BufferPool.GetFrame(entry.second);
                    frame->increment();
                    frame->refOn();
                }
                else {
                    Frame* frame = BufferPool.GetFrame(entry.second);
                    frame->used();
                    frame->refOn();
                    BufferPool.UpdateIndex();
                    frame->SetRequest(operation);
                    BufferPool.SetData(Data, entry.second);
                }
            }
        } 
        else {
            // La página ya está mapeada, actualizar el dato en el buffer pool
            int frameID = PageTable.pageMap[pageID];
            BufferPool.IncremetFrame(frameID);
            BufferPool.DirtyFrame(frameID);
            for (const auto& entry : PageTable.pageMap) {
                if (pageID != entry.first){
                    Frame* frame = BufferPool.GetFrame(entry.second);
                    frame->increment();
                    frame->refOn();
                }
                else {
                    Frame* frame = BufferPool.GetFrame(entry.second);
                    frame->used();
                    frame->refOn();
                    BufferPool.UpdateIndex();
                    frame->SetRequest(operation);
                    BufferPool.SetData(Data, entry.second);
                }
            }
        }
        std::cout << "Desea Modificar? (S/N)" << std::endl;
        char response;
        std::cin >> response;
        if (response == 'S' || response == 's') {
            std::cout << " pagina modificada" << std::endl;
        }
        else if (response == 'N' || response == 'n') {
            std::cout << " pagina no modificada" << std::endl;
        }
    }
}

void BufferManager::releasePage(int pageID) {
    int frameID = PageTable.pageMap[pageID];
    Frame* frame = BufferPool.GetFrame(frameID);

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

        Frame* frame = BufferPool.GetFrame(frameID);
        frame->reset();
        BufferPool.ReleaseFrame(frameID);
        PageTable.UnmapPage(pageID);
        BufferPool.UpdateIndex(); // Opcionalmente actualizar el índice para la próxima política CLOCK

        std::cout << "La pagina " << pageID << " ha sido liberada del marco " << frameID << "." << std::endl;
    }
    else{
        Frame* frame = BufferPool.GetFrame(frameID);
        frame->DecrementCount();
        frame->ReleaseRequest();
    }
}

void BufferManager::printPageTable() {
    std::cout << "# Frame ID\t- Page ID\t- Dirty Bit\t- Pin Count\t- Pinned\t- Last Used\t- Ref. Bit\n";
    for (const auto& entry : PageTable.pageMap) {
        Frame* frame = BufferPool.GetFrame(entry.second);
        std::cout << "# " << entry.second << "\t\t- " << entry.first << "\t\t- "
                  << (frame->GetDirty() ? "1" : "0") << "\t\t- "
                  << frame->GetPinCount() << "\t\t- " << frame->GetIsPinned() << "\t\t- " << frame->GetLastUsed() << "\t\t- " << frame->GetRefBit() << "\t\t\n";
    }
    for (const auto& entry : PageTable.pageMap) {
        Frame* frame = BufferPool.GetFrame(entry.second);
        std::cout << "Los requerimientos del frame " << entry.second << " son:" << std::endl;
        frame->PrintRequest();
    }
}
void BufferManager::PinearPagina(int pageID) {
    auto it = PageTable.pageMap.find(pageID);
    if (it != PageTable.pageMap.end()) {
        int frameID = it->second;
        Frame* frame = BufferPool.GetFrame(frameID);
        frame->pin();
    } else {
        std::cout << "La pagina no se encuentra" << std::endl;
    }
}

void BufferManager::PrintRequest() {
    for (const auto& entry : PageTable.pageMap) {
        Frame* frame = BufferPool.GetFrame(entry.second);
        std::cout << "Los requerimientos del frame " << entry.second << " son:" << std::endl;
        frame->PrintRequest();
    }
}

void BufferManager::ViewPagina(int pageID) {
    std::cout << "\x1B[2J\x1B[H";  // Limpia la pantalla
    auto it = PageTable.pageMap.find(pageID);
    if (it != PageTable.pageMap.end()) {
        int frameID = it->second;
        Frame* frame = BufferPool.GetFrame(frameID);
        std::cout << "El contenido de la pagina " << pageID << " es:" << std::endl; 
        frame->GetData();   
    } else {
        std::cout << "La pagina no se encuentra" << std::endl;
    }  
    
    std::cout << "\nPresione cualquier tecla para continuar...";
    _getch(); // Espera a que el usuario presione una tecla
    std::cout << "\x1B[2J\x1B[H";  // Limpia la pantalla
}
