#include "PageTable.h"
#include <iostream>

PageTable::PageTable() {
}

void PageTable::MapPageToFrame(int pageID, int frameID) {
    pageMap[pageID] = frameID;
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

int PageTable::GetSize(){
    return pageMap.size();
}

