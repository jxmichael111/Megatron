#pragma once
#include <map>

class PageTable {
public:
    std::map<int, int> pageMap;

    int getFrame(int pageID);
    /*
    Esta función busca en la tabla de páginas (pageMap) el ID de marco asociado con un ID de página dado (pageID).
    Si encuentra una entrada correspondiente en el mapa, devuelve el ID de marco asociado. Si no encuentra ninguna 
    entrada para el ID de página dado, devuelve -1 para indicar que la página no está presente en la tabla de páginas.
    */
    void setFrame(int pageID, int frameID);
    /*
    Esta función establece o actualiza la asociación entre un ID de página y un ID de marco en la tabla de 
    páginas (pageMap). Toma el ID de página (pageID) y el ID de marco (frameID) como argumentos y los asigna 
    en el mapa de páginas.
    */
    void removePage(int pageID);
    /*
    Esta función elimina una entrada específica de la tabla de páginas. Toma el ID de página (pageID) como
    argumento y elimina la entrada correspondiente en el mapa de páginas. Si no hay ninguna entrada para 
    el ID de página dado, la función no realiza ninguna acción.
    */
};