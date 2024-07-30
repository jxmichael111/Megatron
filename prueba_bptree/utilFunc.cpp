/*
#include <iostream>
#include <limits>
#include <sstream>

#include "B+ Tree.h"

Node* parent = NULL;

Node::ptr::ptr() {
}

Node::ptr::~ptr() {
}

Node::Node() {
    this->isLeaf = false;
    this->ptr2next = NULL;
}

void Node::serialize(std::ofstream& out) {
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
*/

/* serialize normal
void Node::serialize(std::ofstream& out) {
    out << isLeaf << " " << keys.size() << " ";
    for (int key : keys) {
        out << key << " ";
    }
    if (!isLeaf) {
        out << ptr2TreeOrData.ptr2Tree.size() << " ";
        for (Node* child : ptr2TreeOrData.ptr2Tree) {
            child->serialize(out);
        }
    } else {
        out << ptr2TreeOrData.dataPtr.size() << " ";
        for (FILE* data : ptr2TreeOrData.dataPtr) {
            out << "data_file_placeholder ";
        }
    }
}
*/
/* deserialize normal
Node* Node::deserialize(std::ifstream& in) {
    Node* node = new Node();
    size_t keysSize, childrenSize;
    in >> node->isLeaf >> keysSize;
    node->keys.resize(keysSize);
    for (size_t i = 0; i < keysSize; ++i) {
        in >> node->keys[i];
    }
    if (!node->isLeaf) {
        in >> childrenSize;
        node->ptr2TreeOrData.ptr2Tree.resize(childrenSize);
        for (size_t i = 0; i < childrenSize; ++i) {
            node->ptr2TreeOrData.ptr2Tree[i] = Node::deserialize(in);
        }
    } else {
        in >> childrenSize;
        node->ptr2TreeOrData.dataPtr.resize(childrenSize);
        for (size_t i = 0; i < childrenSize; ++i) {
            std::string placeholder;
            in >> placeholder;
            node->ptr2TreeOrData.dataPtr[i] = nullptr;
        }
    }
    return node;
}
*/

/* deserialize con vista de ejecucion
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
        
        cout << "cant hijos: " << cantLabel << keysSize << endl;

        node->keys.resize(keysSize);

        for (size_t i = 0; i < keysSize; ++i) {
            if (!std::getline(in, line)) break;
            std::istringstream leafStream(line);
            std::string leafLabel, path;
            int key;
            leafStream >> leafLabel >> key >> path;
            cout << "---" << leafLabel << " " << key << " " << path << endl;
            node->keys[i] = key;
            // Depuración
            std::cout << "Deserialized Leaf: " << key << std::endl;
        }
    } else if (nodeType == "Internal") {
        node->isLeaf = false;
        std::string Scant, Sseparador, hijosLabel;
        size_t keysSize, childrenSize;
        iss >> Scant >> keysSize >> Sseparador;
            // cant:       1            |       //del archivo .dat        
        node->keys.resize(keysSize);

        for (size_t i = 0; i < keysSize; ++i) {
            iss >> node->keys[i];
            // Depuración
            std::cout << "Deserialized Internal Key: " << node->keys[i] << std::endl;
        }

        iss >> hijosLabel >> childrenSize;
        cout << childrenSize << endl;
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
*/

/*
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



BPTree::BPTree() {
    this->maxIntChildLimit = 4;
    this->maxLeafNodeLimit = 3;
    this->root = NULL;
}

BPTree::BPTree(int degreeInternal, int degreeLeaf) {
    this->maxIntChildLimit = degreeInternal;
    this->maxLeafNodeLimit = degreeLeaf;
    this->root = NULL;
}

int BPTree::getMaxIntChildLimit() {
    return maxIntChildLimit;
}

int BPTree::getMaxLeafNodeLimit() {
    return maxLeafNodeLimit;
}

Node* BPTree::getRoot() {
    return this->root;
}

void BPTree::setRoot(Node *ptr) {
    this->root = ptr;
}

Node* BPTree::firstLeftNode(Node* cursor) {
    if (cursor->isLeaf)
        return cursor;
    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
        if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL)
            return firstLeftNode(cursor->ptr2TreeOrData.ptr2Tree[i]);

    return NULL;
}
*/
//Node** BPTree::findParent(Node* cursor, Node* child) {
    /*
		Finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
		also ignores second last level because we will never find parent of a leaf node during insertion using this function
	*/
/*
    if (cursor->isLeaf || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf)
        return NULL;

    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
        if (cursor->ptr2TreeOrData.ptr2Tree[i] == child) {
            parent = cursor;
        } else {
            //Commenting To Remove vector out of bound Error: 
            //new (&cursor->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
            Node* tmpCursor = cursor->ptr2TreeOrData.ptr2Tree[i];
            findParent(tmpCursor, child);
        }
    }

    return &parent;
}

void BPTree::serialize(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening file for serialization: " << filename << std::endl;
        return;
    }
    if (root) {
        root->serialize(out);
    }
    out.close();
}

void BPTree::deserialize(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error opening file for deserialization: " << filename << std::endl;
        return;
    }
    root = Node::deserialize(in);
    in.close();
}

void BPTree::toDot(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening file for DOT output: " << filename << std::endl;
        return;
    }

    out << "digraph BPTree {\n";
    out << "node [shape=record];\n";
    int nodeCount = 0;
    if (root) {
        root->toDot(out, nodeCount);
    }
    out << "}\n";
    out.close();
}
*/
