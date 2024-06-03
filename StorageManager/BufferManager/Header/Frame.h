#pragma once
#include <vector>
#include <string>

class Frame {
public:
    int pageID;
    bool isPinned;
    bool dirtyFlag;
    int pinCount;
    int lastUsed;
    std::vector<std::vector<std::string>> content;

    Frame();
    /*
    Constructor de la clase Frame. Inicializa un marco con valores por defecto: el ID de página se establece en -1,
     el marco no está "pinneado" (no está en uso), la bandera de suciedad está desactivada, el contador de "pines" 
     se establece en 0, y el tiempo de "último uso" se establece en 0.
    */
    void pin();
    /*
    Esta función marca el marco como "pinneado", lo que indica que está en uso. Incrementa el contador de "pines" 
    para este marco.
    */
    void unpin();
    /*
    Esta función reduce el contador de "pines" para el marco. Si el contador de "pines" llega a cero, marca el marco 
    como no "pinneado".
    */
    void setDirty(bool flag);
    /*
    Esta función establece la bandera de suciedad para el marco. Toma un valor booleano (flag) como argumento y 
    establece la bandera de suciedad del marco en verdadero o falso según el valor del flag.
    */
    void reset();
    /*
    Esta función restablece todos los atributos del marco a sus valores por defecto. Esto incluye establecer el ID de 
    página en -1, marcar el marco como no "pinneado", desactivar la bandera de suciedad, restablecer el contador de 
    "pines" a 0 y establecer el tiempo de "último uso" en 0.
    */
};