#pragma once
#include <vector>
#include "Frame.h"

class BufferPool {
private:
    std::vector<Frame> bufferFrames;
    int currentIndex;
public:
    BufferPool(int, int);
    int FindUnpinnedFrame();
    std::vector<Frame> GetBufferFrame();
    Frame* GetFrame(int);
    void PinFrame(int);
    void UnpinFrame(int);
    void ReleaseFrame(int);
    void DirtyFrame(int);
    void ResetBufferPool();
    bool AllFramesInUse();

    void UpdateIndex();
    int GetIndex();

    int LRU();
    int CLOCK();

    
};
