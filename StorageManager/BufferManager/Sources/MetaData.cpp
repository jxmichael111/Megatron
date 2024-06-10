#include "MetaData.h"

MetaData::MetaData() {
    this->lastUsed = 0;
    this->refBit = false;
}
void MetaData::increment(){ 
    lastUsed++ 
}

void MetaData::invest() { 
    if (refBit) 
        refBit = false;
    else
        refBit = true;
}

void MetaData::reset() {
    lastUsed = 0;
    refBit = false;
}

int MetaData::getLastUsed(){
    return lastUsed;
}

int MetaData::getRefBit(){
    return refBit;
}
