#pragma once
#include <vector>
#include "Frame.h"

class BufferPool {
private:
    std::vector<Frame> bufferFrames;
    int currentIndex;
    int id;
public:
    BufferPool(int, int);
    int FindUnpinnedFrame();
    std::vector<Frame> GetBufferFrame();
    Frame* GetFrame(int);
    void PinFrame(int);
    void UnpinFrame(int);

    void IncremetFrame(int);
    void DecremetFrame(int);
    
    void ReleaseFrame(int);
    void DirtyFrame(int);
    void ResetBufferPool();
    bool AllFramesInUse();

    void UpdateIndex();
    int GetIndex();
    int GetSize();

    int LRU();
    int CLOCK();

    
};
