#include <iostream>
#include <algorithm>
#include "ReplacementPolicy.h"

ReplacementPolicy::ReplacementPolicy(int size) {
    bufferFrames.reserve(size);
    for (int i = 0; i < size; ++i) {
        bufferFrames.emplace_back(MetaData());
    }
}

int ReplacementPolicy::LRU() {
    int minIndex = 0;
    int minLastUsed = Replace[0].getLastUsed();
    for (int i = 1; i < Replace.size(); ++i) {
        if (replace[i].getLastUsed() < minLastUsed) {
            minLastUsed = Replace[i].getLastUsed();
            minIndex = i;
        }
    }
    return minIndex;
}
// Método para realizar el reemplazo CLOCK
int ReplacementPolicy::CLOCK() {
    while (true) {
        if (!Replace[currentIndex].getRefBit()) {
            return currentIndex;
        } else {
            Replace[currentIndex].invest();
        }
        currentIndex = (currentIndex + 1) % Replace.size(); // Avanzar al siguiente índice circularmente
    }
}