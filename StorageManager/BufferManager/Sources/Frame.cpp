#include "Frame.h"

Frame::Frame(int capacidad_) {
    this->capacidad = capacidad_;
    this->data = "";
    this->isPinned = false ;
    this->dirtyFlag = false ;
    this->pinCount = 0;
}

void Frame::pin() {
    if(!pinCount)
        isPinned = true;
    pinCount++; 
}

void Frame::unpin() {
    if (pinCount > 0) {
        pinCount--; 
        if (pinCount == 0) {
            isPinned = false;
        }
    }
}

void Frame::setDirty() {
    dirtyFlag = true;
}

void Frame::reset() {
    isPinned = false;
    dirtyFlag = false;
    pinCount = 0;
    data.clear();
}

void Frame::SetData(std::string registro) {
    data.push_back(registro);
}

std::vector<std::string> Frame::GetData() {
    return data;
}
