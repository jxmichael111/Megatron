#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <queue>

class Frame {
private:
    int capacidad; 
    int espacio_disponible;
    int espacio_usado;

    std::vector<std::string> data;  // data guarda todo un bloque en memoria
    std::queue<char> requests;      // solicitudes
    bool isPinned;
    bool dirtyFlag;
    int pinCount;
    int lastUsed; //LRU
    bool refBit; //CLOCK


    void ViewEspacio();
    void addCapacidad();

public:
    /*
        @author Michael Ticona
    */
    std::vector<std::string> GetData();
    /*
        @author Michael Ticona
    */
    void ViewRegister(int);

    std::vector<int> GetIndex(int);

    std::vector<std::string> GetRegister(int);

    void SetRegister(int, std::vector<std::string>);

    bool GetDirty();
    
    /*
        @author Michael Ticona
    */
    int GetPinCount();

    /*
        @author Michael Ticona
    */
    bool GetIsPinned();

    /*
        @author Michael Ticona
    */
    int GetLastUsed();

    /*
        @author Michael Ticona
    */
    bool GetRefBit();

    /*
        @author Michael Ticona
    */
    void SetData(std::string);

    /*
        @author Michael Ticona
    */
    void ViewData();

    /*
        @author Michael Ticona
    */
    void SetRequest(char);

    /*
        @author Michael Ticona
    */
    void ReleaseRequest();

    /*
        @author Michael Ticona
    */
    void PrintRequest();

    /*
        @author Michael Ticona
    */
    char GetRequestFront();

    /*
        @author Michael Ticona
    */
    Frame(int);
    /*
    Constructor de la clase Frame. Inicializa un marco con valores por defecto: el ID de página se establece en -1,
    el marco no está "pinneado" (no está en uso), la bandera de suciedad está desactivada, el contador de "pines" 
    se establece en 0, y el tiempo de "último uso" se establece en 0.
    */

    /*
        @author Michael Ticona
    */
    void pin();
    /*
    Esta función marca el marco como "pinneado", lo que indica que está en uso. Incrementa el contador de "pines" 
    para este marco.
    */

    /*
        @author Michael Ticona
    */
    void IncrementCount();

    /*
        @author Michael Ticona
    */
    void DecrementCount();

    /*
        @author Michael Ticona
    */
    void unpin();
    /*
    Esta función reduce el contador de "pines" para el marco. Si el contador de "pines" llega a cero, marca el marco 
    como no "pinneado".
    */

    /*
        @author Michael Ticona
    */
    void setDirty();
    /*
    Establece la bandera en verdadero indicando que se a modificado el frame
    */
    void ReleaseDirty();
    /*
        @author Michael Ticona
    */
    void reset();
    /*
    Esta función restablece todos los atributos del marco a sus valores por defecto.
    */

    /*
        @author Michael Ticona
    */
    void increment();

    /*
        @author Michael Ticona
    */
    void refOn();

    /*
        @author Michael Ticona
    */
    void refOff();

    /*
        @author Michael Ticona
    */
    /*
        @author Michael Ticona
    */
    void used();

    void RemoveRegister(int);

    void AddRegister(const std::vector<std::string>);

    bool isVoid();

    int GetDataSize();
};