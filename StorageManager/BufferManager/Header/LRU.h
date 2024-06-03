#pragma once
#include <list>

class Replacer {
private:
    std::list<int> lruQueue;

public:
    void removePage(int pageID);
    /*
    Elimina una página específica de la lista de páginas menos recientemente utilizadas (LRU).No está 
    implementada en el archivo que proporcionaste, pero probablemente tendría como objetivo eliminar 
    la página pageID de alguna estructura de datos interna.
    */
    int findFrame();
    /*
    Busca y devuelve el ID de un marco disponible en la lista LRU. Si la lista no está vacía, devuelve 
    el ID del primer marco en la lista (que es el menos recientemente utilizado), lo elimina de la lista 
    y lo devuelve. Si la lista está vacía, devuelve -1 para indicar que no hay marcos disponibles.
    */
    void updateLRU(int frameID);
    /*
    Actualiza la posición de un marco dado en la lista LRU. Primero elimina el marco de la 
    lista (si ya está presente) y luego lo vuelve a agregar al final de la lista, lo que lo marca como el 
    más recientemente utilizado.*/
    void addToQueue(int frameID);
    /*
    Agrega un nuevo marco al final de la lista LRU. Recibe como argumento el ID del marco a agregar 
    y simplemente lo coloca al final de la lista.
    */
    void removeFromQueue(int frameID);
    /*
    Elimina un marco específico de la lista LRU. Busca el marco dado en la lista y lo elimina si lo 
    encuentra. Si no se encuentra, no hace nada.
    */
};