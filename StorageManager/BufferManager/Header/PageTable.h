#pragma once
#include <unordered_map>

class PageTable {
public:

    /*
        @author Michael Ticona
    */
    std::unordered_map<int, int> pageMap;

    /*
        @author Michael Ticona
    */
    PageTable();

    /*
        @author Michael Ticona
    */
    void MapPageToFrame(int, int);

    /*
        @author Michael Ticona
    */
    void UnmapPage(int);

    /*
        @author Michael Ticona
    */
    bool IsPageMapped(int) const;

    /*
        @author Michael Ticona
    */
    void ResetPageTable();

    /*
        @author Michael Ticona
    */
    int GetSize();
};