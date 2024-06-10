#pragma once
#include <vector>
#include <string>

class Frame {
private:
    int capacidad; 
    std::vector<std::string> data;
    bool isPinned;
    bool dirtyFlag;
    int pinCount;
public:
    Frame(int);
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
    void setDirty();
    /*
    Establece la bandera en verdadero indicando que se a modificado el frame
    */
    void reset();
    /*
    Esta función restablece todos los atributos del marco a sus valores por defecto.
    */
    void SetData(std::string);
    /*
    */
    std::vector<std::string> GetData();
    /*
    */
};