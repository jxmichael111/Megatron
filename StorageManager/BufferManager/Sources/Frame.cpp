#include <string>
#include "Frame.h"

void Frame::SetPinCount(char operacion) {
    operacion = operacion - ('a' - 'A');
    countPin.push(operacion);
}

void Frame::ReleasePinCount() {
    if (!countPin.empty()) {
        countPin.pop();
    }
}

void Frame::PrintPinCount() {
    std::queue<char> tempQueue = countPin; 
    while (!tempQueue.empty()) {
        char entry = tempQueue.front();
        std::cout << entry << " ";
        tempQueue.pop();
    }
    std::cout<< std::endl;
}

Frame::Frame(int capacidad_)
{
    this->capacidad = capacidad_;
    this->isPinned = false ;
    this->dirtyFlag = false ;
    this->pinCount = 0;
    this->lastUsed = 0;
    this->refBit = false;
}

void Frame::pin() {
        isPinned = true;
}

void Frame::IncrementCount() {
    pinCount++; 
}

void Frame::DecrementCount() {
    if (pinCount > 0) {
        pinCount--; 
//          if (pinCount == 0) {
//              isPinned = false;
//          }
    }
}

void Frame::unpin() {
    isPinned = false;
}

void Frame::setDirty() {
    dirtyFlag = true;
}

void Frame::
reset() {
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

int Frame::GetPinCount() {
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


void Frame::used(){
    lastUsed = 1;
}