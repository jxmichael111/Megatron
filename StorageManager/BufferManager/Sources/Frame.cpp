#include <string>
#include <iostream>
#include "Frame.h"

void Frame::ViewData() {
    for(int i = 0; i < data.size(); i++) {
        std::cout << data[i] << std::endl;
    }
}

void Frame::SetRequest(char operacion)
{   
    if (operacion == 'l' || operacion == 'w')
        operacion = operacion - ('a' - 'A');
    requests.push(operacion);
}

void Frame::ReleaseRequest() {
    if (!requests.empty()) {
        requests.pop();
    }
}

void Frame::PrintRequest() {
    std::queue<char> tempQueue = requests; 
    while (!tempQueue.empty()) {
        char entry = tempQueue.front();
        std::cout << entry << " ";
        tempQueue.pop();
    }
    std::cout<< std::endl;
}

char Frame::GetRequestFront()
{
    return requests.front();
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
    }
}

void Frame::unpin() {
    isPinned = false;
}

void Frame::setDirty() {
    dirtyFlag = true;
}

void Frame::ReleaseDirty() {
    dirtyFlag = false;
}

void Frame::reset() {
    isPinned = false;
    dirtyFlag = false;
    pinCount = 0;
    lastUsed = 0;
    refBit = false;
    data.clear();
    while (!requests.empty()) {
        requests.pop();
    }
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

void Frame::GetData() {
    for(int i = 0; i < data.size(); i++){
        std::cout << data[i] << std::endl;
    }
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

void Frame::used(){
    lastUsed = 1;
}