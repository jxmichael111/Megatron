#pragma once
#include <vector>
#include "Frame.h"

class BufferPool {
private:
    std::vector<Frame> BufferFrame;
public:
    BufferPool(int, int);
    Frame* GetFrame(int frameId);
    void PinFrame(int frameId);
    void UnpinFrame(int frameId);
    void ResetBufferPool();
};
