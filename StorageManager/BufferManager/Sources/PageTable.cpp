#include "PageTable.h"
#include <iostream>

// Constructor por defecto: no es necesario inicializar nada adicional en este caso
PageTable::PageTable() {
}

void PageTable::MapPageToFrame(int pageID, int frameId) {
    pageMap[pageID] = frameId;
}

void PageTable::UnmapPage(int pageID) {
    pageMap.erase(pageID);
}

bool PageTable::IsPageMapped(int pageID) const {
    return pageMap.find(pageID) != pageMap.end();
}

void PageTable::ResetPageTable() {
    pageMap.clear();
}
