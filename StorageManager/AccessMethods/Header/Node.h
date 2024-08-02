#ifndef NODE_H
#define NODE_H

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>


/*
    bool isLeaf: Indica si el nodo es una hoja.
    std::vector<int> keys: Almacena las claves en el nodo.
    union Ptr2TreeOrData:
        std::vector<std::pair<int, int>> dataPtr: Para nodos hoja, almacena pares de enteros que representan datos.
        std::vector<Node*> ptr2Tree: Para nodos internos, almacena punteros a nodos hijos.
    Node* ptr2next: Apunta al siguiente nodo (usado en nodos hoja para formar una lista enlazada).
*/
class Node {
public:
    bool isLeaf;
    std::vector<int> keys;

    union Ptr2TreeOrData {
        std::vector<std::pair<int, int>> dataPtr;  // Para nodos hoja
        std::vector<Node*> ptr2Tree;               // Para nodos internos

        Ptr2TreeOrData() {}
        ~Ptr2TreeOrData() {}
    } ptr2TreeOrData;

    Node* ptr2next;

    Node(bool leaf = false);
    void serialize(std::ofstream& out);
    static Node* deserialize(std::ifstream& in);
    void toDot(std::ofstream& out, int& nodeCount);
};

#endif // NODE_H
