#include <iostream>
#include <cassert>
#include "BufferManager.h"

BufferManager::BufferManager(int BufferSize, int capacidad) : BufferPool(BufferSize, capacidad), PageTable() {}

void BufferManager::requestPage(int pageID, char operation) {
    if (BufferPool.AllFramesInUse()) {
        char strategy;
        std::cout << "El buffer pool esta lleno. Por favor, seleccione la estrategia de reemplazo ('L' para LRU, 'C' para CLOCK): ";
        std::cin >> strategy;      
        if (strategy != 'L' && strategy != 'C') {
            std::cout << "Estrategia de reemplazo no valida. Utilizando LRU por defecto." << std::endl;
            strategy = 'L'; 
        }
        int aux;
        if (strategy == 'L') {
            
            aux = BufferPool.LRU();
            std::cout << aux << std::endl;
            for (const auto& entry : PageTable.pageMap) {
                if(entry.second == aux)
                    aux = entry.first;
            }
            std::cout << aux << std::endl;
            releasePage(aux);
        } else {
            aux = BufferPool.CLOCK();
            for (const auto& entry : PageTable.pageMap) {
                if(entry.second == aux)
                    aux = entry.first;
            }
            releasePage(aux);
        }
    }

    if (operation == 'L' || operation =='l') {
        if (!PageTable.IsPageMapped(pageID)) {
            int frameID = BufferPool.FindUnpinnedFrame();
            PageTable.MapPageToFrame(pageID, frameID);
            BufferPool.PinFrame(frameID);
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
                }
            }
        } else {
            int frameID = PageTable.pageMap[pageID];
            BufferPool.PinFrame(frameID);
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
                }
            }
        }
    }
    else if (operation == 'W' || operation =='w'){
        if (!PageTable.IsPageMapped(pageID)) {
            int frameID = BufferPool.FindUnpinnedFrame();
            PageTable.MapPageToFrame(pageID, frameID);
            BufferPool.PinFrame(frameID);
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
                }
            }
        } else {
            // La página ya está mapeada, actualizar el dato en el buffer pool
            int frameID = PageTable.pageMap[pageID];
            BufferPool.PinFrame(frameID);
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
                }
            }
        }
    }
}

void BufferManager::releasePage(int pageID) {
    int frameID = PageTable.pageMap[pageID];
    Frame* frame = BufferPool.GetFrame(frameID);

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
        frame->unpin();
    }
}



void BufferManager::printPageTable() {
    std::cout << "# Frame ID\t- Page ID\t- Dirty Bit\t- Pin Count\t- Last Used\t- Ref. Bit\n";
    for (const auto& entry : PageTable.pageMap) {
        Frame* frame = BufferPool.GetFrame(entry.second);
        std::cout << "# " << entry.second << "\t\t- " << entry.first << "\t\t- "
                  << (frame->GetDirty() ? "1" : "0") << "\t\t- "
                  << frame->GetPinCount() << "\t\t- " << frame->GetLastUsed() << "\t\t- " << frame->GetRefBit() << "\t\t\n";
    }
}



