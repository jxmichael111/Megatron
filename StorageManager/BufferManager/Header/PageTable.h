#pragma once
#include <unordered_map>

class PageTable {
public:
    std::unordered_map<int, int> pageMap;
    PageTable();
    void MapPageToFrame(int pageId, int frameId);
    void UnmapPage(int pageId);
    bool IsPageMapped(int pageId) const;
    void ResetPageTable();
};