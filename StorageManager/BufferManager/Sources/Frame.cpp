#include <string>
#include "Frame.h"

Frame::Frame(int capacidad_) {
    this->capacidad = capacidad_;
    this->isPinned = false ;
    this->dirtyFlag = false ;
    this->pinCount = 0;
    this->lastUsed = 0;
    this->refBit = false;
}

void Frame::pin() {
    if(!pinCount)
        isPinned = true;
    pinCount++; 
}

void Frame::unpin() {
    if (pinCount > 0) {
        pinCount--; 
//        if (pinCount == 0) {
//            isPinned = false;
//        }
    }
}

void Frame::setDirty() {
    dirtyFlag = true;
}

void Frame::reset() {
    isPinned = false;
    dirtyFlag = false;
    pinCount = 0;
    lastUsed = 0;
    refBit = false;
    data.clear();
}

bool Frame::GetIsPinned() {
    return isPinned;
}

int Frame::GetLastUsed() {
    return lastUsed;
}

bool Frame::GetRefBit() {
    return refBit;
}

void Frame::SetData(std::string registro) {
    data.push_back(registro);
}

std::vector<std::string> Frame::GetData() {
    return data;
}

bool Frame::GetDirty() {
    return dirtyFlag;
}

int Frame::GetPinCount() const{
    return pinCount;
}

void Frame::increment(){ 
    lastUsed++;
}  

void Frame::refOn() {
    refBit = true;
}

void Frame::refOff() {
    refBit = false;
}

int Frame::getLastUsed(){
    return lastUsed;
}

int Frame::getRefBit(){
    return refBit;
}

void Frame::used(){
    lastUsed = 1;
}