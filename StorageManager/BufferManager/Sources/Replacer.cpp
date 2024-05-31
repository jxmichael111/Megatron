#include "Replacer.h"
#include <iostream>

int Replacer::findFrame() {
    if (!lruQueue.empty()) {
        int frameID = lruQueue.front();  // Obten el frame menos recientemente usado
        lruQueue.pop_front();            // Retira el frame de la cola
        return frameID;
    }
    return -1;  // Si la lista esta vacia, indica que no hay frames disponibles
}

void Replacer::updateLRU(int frameID) {
    // Elimina el frame si ya esta en la cola y lo añade al final para marcarlo como el m�s recientemente usado
    removeFromQueue(frameID);
    addToQueue(frameID);
}

void Replacer::addToQueue(int frameID) {
    lruQueue.push_back(frameID);  // Añade el frame ID al final de la cola
    std::cout << "Frame " << frameID << " added to LRU queue.\n";
}

void Replacer::removeFromQueue(int frameID) {
    auto it = std::find(lruQueue.begin(), lruQueue.end(), frameID);
    if (it != lruQueue.end()) {
        lruQueue.erase(it);
        std::cout << "Frame " << frameID << " removed from LRU queue.\n";
    }
}