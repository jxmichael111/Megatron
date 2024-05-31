#pragma once
#include <vector>
#include "Frame.h"

class BufferPool {
private:
    std::vector<Frame> frames;
    //int lastUsedCounter;  // Contador para el seguimiento de �ltimo uso de los frames

public:
    BufferPool(int size);
    /*Autor: Christian Daniel Taipe Saraza
    Constructor de la clase BufferPool. Toma el tamaño del conjunto de marcos como argumento e inicializa el 
    conjunto de marcos con la cantidad especificada de marcos. No solo eso, también inicializa el contador de 
    "último uso" de cada marco a 0.
    */
    void updateCount(int frameID);
    /*Autor: Leonardo Gustavo Gaona Briceño
    Actualizar el pin count de un frame en especifico
    */
    void setPage(int pageID, int frameID);
    /*Autor: Christian Daniel Taipe Saraza
    Esta función establece una página en el marco especificado por su ID (frameID). Verifica si el frameID es válido 
    dentro del rango de marcos disponibles. Si es así, asigna el ID de la página especificada al marco correspondiente 
    y restablece el indicador de suciedad (dirtyFlag) del marco. Luego, actualiza el contador de "último uso" del marco.
    */
    Frame& getFrame(int frameID);
    /*Autor: Leonardo Gustavo Gaona Briceño
    Esta función devuelve una referencia al marco especificado por su ID (frameID). Antes de devolver el marco, 
    actualiza su contador de "último uso".
    */
    void releaseFrame(int frameID);
    /*Autor: Christian Daniel Taipe Saraza
    Esta función libera el marco especificado por su ID (frameID). Al "liberar" un marco, se asigna un marco vacío 
    a ese ID de marco, restableciendo así todos sus valores.
    */
    int size();
    /*Autor: Leonardo Gustavo Gaona Briceño
    Esta función devuelve el tamaño del conjunto de marcos, es decir, la cantidad total de marcos en el conjunto.
    */
    int getLeastRecentlyUsed();
    /*Autor: Michael Jarnie Ticona Larico
    Esta función busca y devuelve el índice del marco menos recientemente utilizado en el conjunto. Itera sobre todos 
    los marcos disponibles, excluyendo aquellos que están "pinneados" (bloqueados), y devuelve el índice del marco 
    con el menor valor de "último uso".
    */
    void updateLastUsed(int frameID);
    /*Autor: Leonardo Gustavo Gaona Briceño
    Esta función actualiza el contador de "último uso" del marco especificado por su ID (frameID). Sin embargo, solo 
    actualiza el contador si el marco no está "pinneado" (bloqueado).
    */
    int getLastUsedCounter();  // Devuelve el contador de �ltimo uso
    /*Autor: Christian Daniel Taipe Saraza
    Esta función devuelve el valor mínimo del contador de "último uso" entre todos los marcos. Se utiliza para determinar 
    qué marco ha sido el menos recientemente utilizado. Sin embargo, parece que hay un error en esta función, ya que 
    debería verificar si los marcos están "pinneados" antes de considerar su "último uso".
    */
    void increment(int);
};
