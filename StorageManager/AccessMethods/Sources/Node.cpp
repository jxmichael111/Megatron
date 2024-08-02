#include "Node.h"

Node::Node(bool leaf) {
    this->isLeaf = leaf;
    this->ptr2next = NULL;
    if (leaf) {
        ptr2TreeOrData.dataPtr = std::vector<std::pair<int, int>>();
    } else {
        ptr2TreeOrData.ptr2Tree = std::vector<Node*>();
    }
}

void Node::serialize(std::ofstream& out) {
    if (isLeaf) {
        out << "Leaf cant: " << keys.size() << " |\n";
        for (size_t i = 0; i < keys.size(); ++i) {
            out << "Leaf_place " << keys[i] << " " << ptr2TreeOrData.dataPtr[i].first << " " << ptr2TreeOrData.dataPtr[i].second << "\n";
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
        return nullptr;  // Fin del archivo o error
    }
    std::istringstream iss(line);
    std::string nodeType;
    iss >> nodeType;
    Node* node = new Node(nodeType == "Leaf");
    if (nodeType == "Leaf") {
        node->isLeaf = true;
        std::string cantLabel;
        size_t keysSize;
        iss >> cantLabel >> keysSize;
        
        node->keys.resize(keysSize);
        node->ptr2TreeOrData.dataPtr.resize(keysSize);
        for (size_t i = 0; i < keysSize; ++i) {
            if (!std::getline(in, line)) break;
            std::istringstream leafStream(line);
            std::string leafLabel;
            int key, block, line;
            leafStream >> leafLabel >> key >> block >> line;
            node->keys[i] = key;
            node->ptr2TreeOrData.dataPtr[i] = std::make_pair(block, line);
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
                std::cerr << "Error deserializando nodo hijo" << std::endl;
                break;
            }
        }
    } else {
        std::cerr << "Tipo de nodo desconocido: " << nodeType << std::endl;
        delete node;
        return nullptr;
    }
    return node;
}

void Node::toDot(std::ofstream& out, int& nodeCount) {
    int currentNode = nodeCount++;
    out << "node" << currentNode << " [label=\"";
    for (size_t i = 0; i < keys.size(); ++i) {
        out << "<f" << i << "> " << keys[i];
        if (i != keys.size() - 1) {
            out << " | ";
        }
    }
    out << " | <f" << keys.size() << ">\"];\n";
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
