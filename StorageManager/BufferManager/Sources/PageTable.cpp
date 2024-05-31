#include "PageTable.h"

int PageTable::getFrame(int pageID) {
    // Devuelve el Frame ID asociado con el Page ID dado, o -1 si no se encuentra.
    auto it = pageMap.find(pageID);
    if (it != pageMap.end()) {
        return it->second;
    } // lineups
    return -1;  // Retorna -1 si la página no está presente
}

void PageTable::setFrame(int pageID, int frameID) {
    // Asocia un Page ID con un Frame ID en la tabla de páginas.
    pageMap[pageID] = frameID;  // Establece o actualiza el mapeo
}

void PageTable::removePage(int pageID) {
    pageMap.erase(pageID);
}