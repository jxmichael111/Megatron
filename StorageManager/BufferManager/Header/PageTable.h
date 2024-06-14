#pragma once
#include <unordered_map>

class PageTable {
public:
    std::unordered_map<int, int> pageMap;
    PageTable();
    void MapPageToFrame(int, int);
    void UnmapPage(int);
    bool IsPageMapped(int) const;
    void ResetPageTable();
    int GetSize();
};