#include "BufferPool.h"
#include <iostream>

BufferPool::BufferPool(int size, int CapacityFrame) {
    this->currentIndex = 0;
    bufferFrames.reserve(size);
    for (int i = 0; i < size; ++i) {
        bufferFrames.emplace_back(Frame(CapacityFrame)); // o con el tamaño apropiado
    }
}

int BufferPool::FindUnpinnedFrame() {
    for (int i = 0; i < bufferFrames.size(); ++i) {
        if (!bufferFrames[i].GetPinCount() && !bufferFrames[i].GetRefBit()) {
            return i;
        }
    }
    return -1;
}

std::vector<Frame> BufferPool::GetBufferFrame() {
    return bufferFrames;
}

Frame* BufferPool::GetFrame(int frameID) {
    if (frameID >= 0 && frameID < bufferFrames.size()) {
        return &bufferFrames[frameID];
    }
    std::cout<<"No se encontro el Frame"<<std::endl;
    return nullptr;
}

void BufferPool::SetData(std::vector<std::string> Data, int frameID) {
    Frame* frame = GetFrame(frameID);
    if (frame) {
        for (int i = 0; i < Data.size(); i++){
            frame->SetData(Data[i]);
        }
    }
}

void BufferPool::mostrarData(int frameID) {
    Frame* frame = GetFrame(frameID);
    if (frame) {
        frame->ViewData();
    }
}

void BufferPool::PinFrame(int frameID) {
    Frame* frame = GetFrame(frameID);
    if (frame) {
        frame->pin();
    }
}

void BufferPool::UnpinFrame(int frameID) {
    Frame* frame = GetFrame(frameID);
    if (frame) {
        frame->unpin();
    }
}

void BufferPool::IncremetFrame(int frameID) {
    Frame* frame = GetFrame(frameID);
    if (frame) {
        frame->IncrementCount();
    }   
}

void BufferPool::DecremetFrame(int frameID) {
    Frame* frame = GetFrame(frameID);
    if (frame) {
        frame->DecrementCount();
    }
}

void BufferPool::ReleaseFrame(int frameID)
{
    Frame* frame = GetFrame(frameID);
    if (frame) {
        frame->reset();
    }
}

void BufferPool::DirtyFrame(int frameID){
    Frame* frame = GetFrame(frameID);
    if (frame) {
        frame->setDirty();
    }    
}

void BufferPool::ResetBufferPool() {
    for (auto& frame : bufferFrames) {
        frame.reset();
    }
}

bool BufferPool::AllFramesInUse() {
    for (auto& frame : bufferFrames) {
        if (!frame.GetPinCount()) {
            return false;
        }
    }
    return true;
}

void BufferPool::UpdateIndex() {
    currentIndex = (currentIndex + 1) % bufferFrames.size();
}

int BufferPool::GetIndex() {
    return currentIndex;
}

int BufferPool::GetSize() {
    return bufferFrames.size();
}

int BufferPool::LRU() {
    int maxIndex = -1;
    int maxLastUsed = 0; 

    for (int i = 0; i < bufferFrames.size(); ++i) {
        if (bufferFrames[i].GetPinCount() == 0 && bufferFrames[i].GetLastUsed() > maxLastUsed) {
            maxLastUsed = bufferFrames[i].GetLastUsed();
            maxIndex = i;
        }
    }

    if (maxIndex == -1) {
        std::cout << "Error: No se encontró un marco no pinneado.\n";
    }

    return maxIndex;
}

// Método para realizar el reemplazo CLOCK
int BufferPool::CLOCK() {
    while (true) {
        Frame &currentFrame = bufferFrames[currentIndex];
        if (!currentFrame.GetRefBit() && currentFrame.GetPinCount() == 0) {
            return currentIndex;
        } else {
            currentFrame.refOff();
        }
        UpdateIndex(); 
    }
}


/*
int BufferManager::MRU(std::vector<Frame>& frames) {
    int minIndex = 0;
    int minLastUsed = Replace[0].getLastUsed();
    for (int i = 1; i < Replace.size(); ++i) {
        if (Replace[i].getLastUsed() > minLastUsed) {
            minLastUsed = Replace[i].getLastUsed();
            minIndex = i;
        }
    }
    return minIndex;
}
*/