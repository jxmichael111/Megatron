#pragma once
#include <vector>
#include "MetaData.h"

class ReplacementPolicy {
private:
    std::vector<MetaData> Replace;
    int currentIndex;

public:
    ReplacementPolicy(int);
    int LRU();
    int CLOCK();
};