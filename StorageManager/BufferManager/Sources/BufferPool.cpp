#include "BufferPool.h"
#include <iostream>

BufferPool::BufferPool(int size, int CapacityFrame) {
    bufferFrames.reserve(size);
    for (int i = 0; i < size; ++i) {
        bufferFrames.emplace_back(Frame(CapacityFrame)); // o con el tamaño apropiado
    }
}

Frame* BufferPool::GetFrame(int frameID) {
    if (frameID >= 0 && frameID < bufferFrames.size()) {
        return &bufferFrames[frameID];
    }
    std::cout<<"No se encontro el Frame"<<std::endl;
    return nullptr;
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

void BufferPool::ResetBufferPool() {
    for (auto& frame : bufferFrames) {
        frame.reset();
    }
}