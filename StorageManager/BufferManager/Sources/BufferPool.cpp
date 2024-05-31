#include "BufferPool.h"

BufferPool::BufferPool(int size) : frames(size) {
    // Inicializa el buffer pool con un tama�o fijo y el contador de �ltimo uso a 0.
}

void BufferPool::updateCount(int frameID) {
    if (frameID < frames.size()) {
        frames[frameID].pin();  // Incrementa y asigna el nuevo valor de último uso.
    }
}
void BufferPool::setPage(int pageID, int frameID) {
    if (frameID < frames.size()) {
        frames[frameID].pageID = pageID;
        frames[frameID].dirtyFlag = false;  // Reinicia el flag de dirty
        updateLastUsed(frameID);  // Actualiza el contador de �ltimo uso
    }
}

Frame& BufferPool::getFrame(int frameID) {
    //updateLastUsed(frameID);  // Actualiza cada vez que se accede a un frame
    return frames[frameID];
}

void BufferPool::releaseFrame(int frameID) {
    frames[frameID] = Frame();
}

int BufferPool::size() {
    return frames.size();
}

int BufferPool::getLeastRecentlyUsed() {
    int lruIndex = -1;
    int minLastUsed = INT_MAX;  // Inicializa con el m�ximo valor posible
    for (int i = 0; i < frames.size(); ++i) {
        if (frames[i].pinCount == 0 && frames[i].lastUsed < minLastUsed) {
            minLastUsed = frames[i].lastUsed;
            lruIndex = i;
        }
    }
    return lruIndex;  // Devuelve el �ndice del frame menos recientemente usado
}

void BufferPool::updateLastUsed(int frameID) {
    if (frameID < frames.size() /*&& !frames[frameID].isPinned*/) { // Solo actualiza si el frame no est� pinned
        frames[frameID].lastUsed++;
    }
}
    
int BufferPool::getLastUsedCounter() { // falta comprobar que sea 0 isPinned tmbn
    int minLastUsed = frames[0].lastUsed;
    for (int i = 1; i < frames.size(); ++i) { 
        if (frames[i].lastUsed < minLastUsed) {
            minLastUsed = frames[i].lastUsed; 
        }
    }
    return minLastUsed; 
}

void BufferPool::increment(int frameID)
{
    for(int i = 0; i < frames.size(); i++){
        if(i != frameID && frames[i].pinCount)
            frames[i].lastUsed++;
    }
}
