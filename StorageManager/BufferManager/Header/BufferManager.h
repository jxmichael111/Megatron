
#pragma once
#include "BufferPool.h"
#include "PageTable.h"
#include "Replacer.h"
#include <string>

class BufferManager {
private:
    BufferPool bufferPool;
    PageTable pageTable;
    Replacer replacer;

public:
    BufferManager(int size);
    /*Autor: Michael Jarnie Ticona Larico
    Constructor de la clase BufferManager. Toma el tamaño del búfer como argumento e inicializa el búfer, la tabla
    de páginas y el reemplazador. Además, agrega todos los marcos disponibles al reemplazador inicialmente.
    */
    void setPage(int pageID);
    /*Autor: Michael Jarnie Ticona Larico
    Esta función se encarga de establecer una página en la memoria si no está presente. Primero intenta obtener
    el ID de marco asociado con el ID de página dado de la tabla de páginas. Si la página ya está en memoria,
    imprime un mensaje indicando que la página ya está en memoria. Si no está en memoria, intenta encontrar un 
    marco disponible utilizando el reemplazador. Si no hay marcos disponibles, imprime un mensaje indicando que 
    no hay marcos disponibles. Si encuentra un marco disponible, verifica si la página actual en el marco está 
    marcada como sucia; si lo está, escribe la página en el disco. Luego, reinicia el marco, establece la nueva 
    página en el marco, actualiza la tabla de páginas y fija el marco como "pin". Finalmente, imprime un mensaje 
    indicando que la página se cargó en el marco.
    */
    Frame& requestPage(int pageID);
    /*Autor: Leonardo Gustavo Gaona Briceño
    Esta función solicita una página con el ID dado. Primero, intenta obtener el ID de marco asociado con el ID de 
    página de la tabla de páginas. Si la página no está en memoria, la carga utilizando la función setPage(). 
    Luego, obtiene el marco correspondiente al ID de marco obtenido y lo "fija" (pin). Devuelve una referencia al 
    marco solicitado.
    */
    void releasePage(int pageID);
    /*Autor: Leonardo Gustavo Gaona Briceño
    Esta función libera una página con el ID dado. Primero, obtiene el ID de marco asociado con el ID de página de 
    la tabla de páginas. Luego, reduce el contador de "pin" del marco asociado. Si el contador de "pin" llega a cero, 
    agrega el marco al reemplazador para un posible reemplazo.
    */
    bool checkPage(int pageID);
    /*Autor: Michael Jarnie Ticona Larico
    Esta función verifica si una página con el ID dado está en la memoria. Devuelve verdadero si la página está en 
    memoria y falso si no lo está.
    */
    void printPageTable();
    /*Autor: Michael Jarnie Ticona Larico
    Esta función imprime la tabla de páginas junto con información relevante sobre cada entrada, como el ID de marco, 
    el ID de página, el bit de suciedad, el recuento de "pin" y el último uso.
    */
};