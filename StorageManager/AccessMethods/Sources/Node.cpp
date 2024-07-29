#include "Node.h"

Node::Node(){
    this->isLeaf = false;
    this->ptr2next = NULL;
}

void Node::serialize(std::ofstream& out){
    if (isLeaf) {
        out << "Leaf cant: " << keys.size() << " |\n";
        for (size_t i = 0; i < keys.size(); ++i) {
            out << "Leaf_place " << keys[i] << " path\n";  // Reemplaza "path" con información de ruta real si es necesario
        }
    } else {
        out << "Internal cant: " << keys.size() << " |";
        for (int key : keys) {
            out << " " << key;
        }
        out << " cant_hijos: " << ptr2TreeOrData.ptr2Tree.size() << "\n";
        for (Node* child : ptr2TreeOrData.ptr2Tree) {
            if (child) {
                child->serialize(out);
            }
        }
    }
}
Node* Node::deserialize(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line)) {
        return nullptr;  // End of file or error
    }
    std::istringstream iss(line);
    std::string nodeType;
    iss >> nodeType;
    Node* node = new Node();
    if (nodeType == "Leaf") {
        node->isLeaf = true;
        std::string cantLabel;
        size_t keysSize;
        iss >> cantLabel >> keysSize;
        
        node->keys.resize(keysSize);
        for (size_t i = 0; i < keysSize; ++i) {
            if (!std::getline(in, line)) break;
            std::istringstream leafStream(line);
            std::string leafLabel, path;
            int key;
            leafStream >> leafLabel >> key >> path;
            node->keys[i] = key;
        }
    } else if (nodeType == "Internal") {
        node->isLeaf = false;
        std::string Scant, Sseparador, hijosLabel;
        size_t keysSize, childrenSize;
        iss >> Scant >> keysSize >> Sseparador;
        node->keys.resize(keysSize);
        for (size_t i = 0; i < keysSize; ++i) {
            iss >> node->keys[i];
        }
        iss >> hijosLabel >> childrenSize;
        node->ptr2TreeOrData.ptr2Tree.resize(childrenSize);
        for (size_t i = 0; i < childrenSize; ++i) {
            node->ptr2TreeOrData.ptr2Tree[i] = Node::deserialize(in);
            if (!node->ptr2TreeOrData.ptr2Tree[i]) {
                std::cerr << "Error deserializing child node" << std::endl;
                break;
            }
        }
    } else {
        std::cerr << "Unknown node type: " << nodeType << std::endl;
        delete node;
        return nullptr;
    }
    return node;
}

void Node::toDot(std::ofstream& out, int& nodeCount) {
    int currentNode = nodeCount++;
    out << "node" << currentNode << " [label=\"";
    // Define los puertos para cada key en el nodo
    for (size_t i = 0; i < keys.size(); ++i) {
        out << "<f" << i << "> " << keys[i];
        if (i != keys.size() - 1) {
            out << " | ";
        }
    }
    // Añadir un puerto extra al final para facilitar las conexiones
    out << " | <f" << keys.size() << ">\"];\n";
    // Conexiones a los hijos si no es un nodo hoja
    if (!isLeaf) {
        for (size_t i = 0; i < ptr2TreeOrData.ptr2Tree.size(); ++i) {
            Node* child = ptr2TreeOrData.ptr2Tree[i];
            if (child) {
                int childNode = nodeCount;
                child->toDot(out, nodeCount);
                out << "node" << currentNode << ":f" << i << " -> node" << childNode << ";\n";
            }
        }
    }
}
