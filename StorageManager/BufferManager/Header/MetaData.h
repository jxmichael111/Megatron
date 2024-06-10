#pragma once

class MetaData {
private:
    int lastUsed; //LRU
    bool refBit; //CLOCK
public:
    MetaData();
    void increment();
    void invest();
    void reset();
    int getLastUsed();
    int getRefBit();
};
