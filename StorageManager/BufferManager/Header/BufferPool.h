#pragma once
#include <vector>
#include "Frame.h"

class BufferPool {
private:
    std::vector<Frame> bufferFrames;
    int currentIndex;
    int id;
public:

    /*
        @author Michael Ticona
    */
    BufferPool(int, int);
    
    /*
        @author Michael Ticona
    */
    int FindUnpinnedFrame();

    /*
        @author Michael Ticona
    */
    std::vector<Frame> GetBufferFrame();

    /*
        @author Michael Ticona
    */
    Frame* GetFrame(int);

    /*
        @author Michael Ticona
    */
    void SetData(std::vector<std::string>, int);

    /*
        @author Michael Ticona
    */
    void mostrarData(int);

    /*
        @author Michael Ticona
    */
    void PinFrame(int);

    /*
        @author Michael Ticona
    */
    void UnpinFrame(int);

    /*
        @author Michael Ticona
    */
    void IncremetFrame(int);

    /*
        @author Michael Ticona
    */
    void DecremetFrame(int);

    /*
        @author Michael Ticona
    */    
    void ReleaseFrame(int);

    /*
        @author Michael Ticona
    */
    void DirtyFrame(int);

    /*
        @author Michael Ticona
    */
    void ResetBufferPool();

    /*
        @author Michael Ticona
    */
    bool AllFramesInUse();

    /*
        @author Michael Ticona
    */

    void UpdateIndex();

    /*
        @author Michael Ticona
    */
    int GetIndex();

    /*
        @author Michael Ticona
    */
    int GetSize();

    /*
        @author Michael Ticona
    */
    int LRU();

    /*
        @author Michael Ticona
    */
    int CLOCK();

    std::vector<std::string> GetData(int);

    
};
