#include "BPTree.h"

Node* parent = NULL;

void BPTree::insertInternal(int x, Node** cursor, Node** child) {
    if ((*cursor)->keys.size() < maxIntChildLimit - 1) {
        int idx = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
        (*cursor)->keys.insert((*cursor)->keys.begin() + idx, x);
        (*cursor)->ptr2TreeOrData.ptr2Tree.insert((*cursor)->ptr2TreeOrData.ptr2Tree.begin() + idx + 1, *child);
    } else {
        std::vector<int> virtualKey((*cursor)->keys);
        std::vector<Node*> virtualTreePtr((*cursor)->ptr2TreeOrData.ptr2Tree);
        int idx = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
        virtualKey.insert(virtualKey.begin() + idx, x);
        virtualTreePtr.insert(virtualTreePtr.begin() + idx + 1, *child);
        int partitionKey = virtualKey[(virtualKey.size() / 2)];
        Node* newInternal = new Node(false);
        (*cursor)->keys.resize((virtualKey.size() / 2));
        (*cursor)->ptr2TreeOrData.ptr2Tree.resize((virtualKey.size() / 2) + 1);
        for (int i = 0; i < (*cursor)->keys.size(); ++i) {
            (*cursor)->keys[i] = virtualKey[i];
        }
        for (int i = 0; i < (*cursor)->ptr2TreeOrData.ptr2Tree.size(); ++i) {
            (*cursor)->ptr2TreeOrData.ptr2Tree[i] = virtualTreePtr[i];
        }
        for (int i = (*cursor)->keys.size() + 1, j = 0; i < virtualKey.size(); ++i, ++j) {
            newInternal->keys.push_back(virtualKey[i]);
        }
        for (int i = (*cursor)->ptr2TreeOrData.ptr2Tree.size(), j = 0; i < virtualTreePtr.size(); ++i, ++j) {
            newInternal->ptr2TreeOrData.ptr2Tree[j] = virtualTreePtr[i];
        }
        if ((*cursor) == root) {
            Node* newRoot = new Node(false);
            newRoot->keys.push_back(partitionKey);
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(*cursor);
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternal);
            root = newRoot;
        } else {
            insertInternal(partitionKey, findParent(root, *cursor), &newInternal);
        }
    }
}

Node** BPTree::findParent(Node* cursor, Node* child) {
    if (cursor->isLeaf || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf) {
        return nullptr;
    }
    for (Node* node : cursor->ptr2TreeOrData.ptr2Tree) {
        if (node == child) {
            return &cursor;
        } else {
            Node** parent = findParent(node, child);
            if (parent != nullptr) {
                return parent;
            }
        }
    }
    return nullptr;
}

Node* BPTree::firstLeftNode(Node* cursor) {
    if (cursor == NULL) {
        return NULL;
    }
    while (cursor->isLeaf == false) {
        cursor = cursor->ptr2TreeOrData.ptr2Tree[0];
    }
    return cursor;
}






BPTree::BPTree(){
    this->maxIntChildLimit = 4;
    this->maxLeafNodeLimit = 3;
    this->root = NULL;
}
BPTree::BPTree(int degreeInternal, int degreeLeaf){
    this->maxIntChildLimit = degreeInternal;
    this->maxLeafNodeLimit = degreeLeaf;
    this->root = NULL;
}
Node* BPTree::getRoot(){
    return this->root;
}
int BPTree::getMaxIntChildLimit(){
    return maxIntChildLimit;
}
int BPTree::getMaxLeafNodeLimit(){
    return maxLeafNodeLimit;
}
void BPTree::setRoot(Node *ptr) {
    this->root = ptr;
}
void BPTree::display(Node* cursor) {
    if (cursor == NULL) return;
    std::queue<Node*> q;
    q.push(cursor);
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node* u = q.front(); q.pop();
            for (int val : u->keys)
                std::cout << val << " ";
            std::cout << "|| ";
            
            if (!u->isLeaf) {
                for (Node* v : u->ptr2TreeOrData.ptr2Tree) {
                    q.push(v);
                }
            }
        }
        std::cout << std::endl;
    }
}

void BPTree::seqDisplay(Node* cursor) {
    Node* firstLeft = firstLeftNode(cursor);
    if (firstLeft == NULL) {
        std::cout << "No hay datos, arbol vacio ...!" << std::endl;
        return;
    }
    while (firstLeft != NULL) {
        for (int i = 0; i < firstLeft->keys.size(); i++) {
            std::cout << firstLeft->keys[i] << " ";
        }
        firstLeft = firstLeft->ptr2next;
    }
    std::cout << std::endl;
}

std::pair<int, int> BPTree::search(int key) {
    if (root == NULL) {
        std::cout << "NO Tuples Inserted yet" << std::endl;
        return std::make_pair(-1, -1);  // Indicador de que no se encontró la clave
    } else {
        Node* cursor = root;
        while (cursor->isLeaf == false) {
            int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
        }
        int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
        if (idx == cursor->keys.size() || cursor->keys[idx] != key) {
            std::cout << "No se encontro ese key" << std::endl;
            return std::make_pair(-1, -1);  // Indicador de que no se encontró la clave
        } else {
            return cursor->ptr2TreeOrData.dataPtr[idx];
        }
    }
}


void BPTree::insert(int key, int NroBloque, int NroRegistro) {
    if (root == NULL) {
        root = new Node(true);
        root->keys.push_back(key);
        root->ptr2TreeOrData.dataPtr.push_back(std::make_pair(NroBloque, NroRegistro));  
        //root->ptr2TreeOrData.dataPtr.push_back(std::make_pair(1, 0));  // Suponiendo el primer bloque y primera línea
        return;
    } else {
        Node* cursor = root;
        Node* parent = NULL;
        while (cursor->isLeaf == false) {
            parent = cursor;
            int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
        }
        if (cursor->keys.size() < maxLeafNodeLimit) {
            int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor->keys.insert(cursor->keys.begin() + idx, key);
            cursor->ptr2TreeOrData.dataPtr.insert(cursor->ptr2TreeOrData.dataPtr.begin() + idx, std::make_pair(NroBloque, NroRegistro) );
            return;
        } else {
            std::vector<int> virtualNode(cursor->keys);
            std::vector<std::pair<int, int>> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);
            int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            virtualNode.insert(virtualNode.begin() + idx, key);
            virtualDataNode.insert(virtualDataNode.begin() + idx, std::make_pair(NroBloque, NroRegistro));

            Node* newLeaf = new Node(true);
            cursor->keys.resize((maxLeafNodeLimit + 1) / 2);
            cursor->ptr2TreeOrData.dataPtr.resize((maxLeafNodeLimit + 1) / 2);
            for (int i = 0; i < cursor->keys.size(); ++i) {
                cursor->keys[i] = virtualNode[i];
                cursor->ptr2TreeOrData.dataPtr[i] = virtualDataNode[i];
            }
            for (int i = 0, j = cursor->keys.size(); j < virtualNode.size(); ++i, ++j) {
                newLeaf->keys.push_back(virtualNode[j]);
                newLeaf->ptr2TreeOrData.dataPtr.push_back(virtualDataNode[j]);
            }
            newLeaf->ptr2next = cursor->ptr2next;
            cursor->ptr2next = newLeaf;
            if (cursor == root) {
                Node* newRoot = new Node(false);
                newRoot->keys.push_back(newLeaf->keys[0]);
                // new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
                root = newRoot;
            } else {
                insertInternal(newLeaf->keys[0], &parent, &newLeaf);
            }
        }
    }
}

void BPTree::removeKey(int x){
    Node* root = getRoot();
    // Si el arbol esta vacio
    if (root == NULL) {
        cout << "B+ Tree is Empty" << endl;
        return;
    }
    Node* cursor = root;
    Node* parent;
    int leftSibling, rightSibling;
    // Ir al nodo hoja que puede contener la clave
    while (cursor->isLeaf != true) {
        for (int i = 0; i < cursor->keys.size(); i++) {
            parent = cursor;
            leftSibling = i - 1;//left side of the parent node
            rightSibling = i + 1;// right side of the parent node
            if (x < cursor->keys[i]) {
                cursor = cursor->ptr2TreeOrData.ptr2Tree[i];
                break;
            }
            if (i == cursor->keys.size() - 1) {
                leftSibling = i;
                rightSibling = i + 2;// CHECK here , might need to make it negative
                cursor = cursor->ptr2TreeOrData.ptr2Tree[i+1];
                break;
            }
        }
    }
    // Comprobar si el valor existe en este nodo hoja
    int pos = 0;
    bool found = false;
    for (pos = 0; pos < cursor->keys.size(); pos++) {
        if (cursor->keys[pos] == x) {
            found = true;
            break;
        }
    }

    if (found == false) {
        cout << "Key Not Found in the Tree" << endl;
        return;
    }
    
    // Eliminar la clave y el par asociado en dataPtr
    for (int i = pos; i < cursor->keys.size()-1; i++) {
        cursor->keys[i] = cursor->keys[i+1];
        cursor->ptr2TreeOrData.dataPtr[i] = cursor->ptr2TreeOrData.dataPtr[i + 1];
    }
    int prev_size = cursor->keys.size();
    cursor->keys.resize(prev_size - 1);
    cursor->ptr2TreeOrData.dataPtr.resize(prev_size - 1);
    
    // Si es hoja y también el nodo raíz
    if (cursor == root) {
        if (cursor->keys.size() == 0) {
            setRoot(NULL);
            cout << "Ohh!! Our Tree is Empty Now :(" << endl;
        }
    }
    
    cout << "Deleted " << x << " From Leaf Node successfully" << endl;
    if ((cursor->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2) || (cursor == root)) {
        // Nodo suficiente para mantener la invariante
        return;
    }
    cout << "UnderFlow in the leaf Node Happended" << endl;
    cout << "Starting Redistribution..." << endl;
    
    // 1. Intentar tomar prestada una clave del hermano izquierdo
    if (leftSibling >= 0) {
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
        // Comprobar si el hermano izquierdo tiene una clave extra para transferir
        if (leftNode->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2 + 1) {
            // Transferir la clave máxima del hermano izquierdo
            int maxIdx = leftNode->keys.size() - 1;
            cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
            cursor->ptr2TreeOrData.dataPtr.insert(cursor->ptr2TreeOrData.dataPtr.begin(), leftNode->ptr2TreeOrData.dataPtr[maxIdx]);
            // Redimensionar el nodo hermano izquierdo después de la transferencia
            leftNode->keys.resize(maxIdx);
            leftNode->ptr2TreeOrData.dataPtr.resize(maxIdx);
            // Actualizar el padre
            parent->keys[leftSibling] = cursor->keys[0];
            cout << "Transferred from left sibling of leaf node" << endl;
            return;
        }
    }
    // 2. Intentar tomar prestada una clave del hermano derecho
    if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
        // Comprobar si el hermano derecho tiene una clave extra para transferir
        if (rightNode->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2 + 1) {
            // Transferir la clave mínima del hermano derecho
            int minIdx = 0;
            cursor->keys.push_back(rightNode->keys[minIdx]);
            cursor->ptr2TreeOrData.dataPtr.push_back(rightNode->ptr2TreeOrData.dataPtr[minIdx]);
            // Redimensionar el nodo hermano derecho después de la transferencia
            rightNode->keys.erase(rightNode->keys.begin());
            rightNode->ptr2TreeOrData.dataPtr.erase(rightNode->ptr2TreeOrData.dataPtr.begin());
            // Actualizar el padre
            parent->keys[rightSibling - 1] = rightNode->keys[0];
            cout << "Transferred from right sibling of leaf node" << endl;
            return;
        }
    }

    // Fusión y eliminación del nodo
    if (leftSibling >= 0) { // Si existe hermano izquierdo
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
        // Transferir clave y dataPtr al hermano izquierdo y conectar ptr2next
        for (int i = 0; i < cursor->keys.size(); i++) {
            leftNode->keys.push_back(cursor->keys[i]);
            leftNode->ptr2TreeOrData.dataPtr.push_back(cursor->ptr2TreeOrData.dataPtr[i]);
        }
        leftNode->ptr2next = cursor->ptr2next;
        cout << "Merging two leaf Nodes" << endl;
        removeInternal(parent->keys[leftSibling], parent, cursor); // eliminar la clave del nodo padre
        delete cursor;
    } else if (rightSibling <= parent->keys.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
        // Transferir clave y dataPtr al hermano derecho y conectar ptr2next
        for (int i = 0; i < rightNode->keys.size(); i++) {
            cursor->keys.push_back(rightNode->keys[i]);
            cursor->ptr2TreeOrData.dataPtr.push_back(rightNode->ptr2TreeOrData.dataPtr[i]);
        }
        cursor->ptr2next = rightNode->ptr2next;
        cout << "Merging two leaf Nodes" << endl;
        removeInternal(parent->keys[rightSibling - 1], parent, rightNode); // eliminar la clave del nodo padre
        delete rightNode;
    }
}

void BPTree::removeInternal(int x, Node* cursor, Node* child){
    Node* root = getRoot();
    // Check if key from root is to deleted
    if (cursor == root) {
        if (cursor->keys.size() == 1) {
            // If only one key is left and matches with one of the
            // child Pointers
            if (cursor->ptr2TreeOrData.ptr2Tree[1] == child) {
                setRoot(cursor->ptr2TreeOrData.ptr2Tree[0]);
                //delete cursor;
                cout << "Wow! New Changed Root" <<endl;
                return;
            }
            else if (cursor->ptr2TreeOrData.ptr2Tree[0] == child) {
                setRoot(cursor->ptr2TreeOrData.ptr2Tree[1]);
                //delete cursor;
                cout << "Wow! New Changed Root" << endl;
                return;
            }
        }
    }
    // Deleting key x from the parent
    int pos;
    for (pos = 0; pos < cursor->keys.size(); pos++) {
        if (cursor->keys[pos] == x) {
            break;
        }
    }
    for (int i = pos; i < cursor->keys.size()-1; i++) {
        cursor->keys[i] = cursor->keys[i + 1];
    }
    cursor->keys.resize(cursor->keys.size() - 1);
    // Now deleting the ptr2tree
    for (pos = 0; pos < cursor->ptr2TreeOrData.ptr2Tree.size(); pos++) {
        if (cursor->ptr2TreeOrData.ptr2Tree[pos] == child) {
            break;
        }
    }
    for (int i = pos; i < cursor->ptr2TreeOrData.ptr2Tree.size() - 1; i++) {
        cursor->ptr2TreeOrData.ptr2Tree[i] = cursor->ptr2TreeOrData.ptr2Tree[i + 1];
    }
    cursor->ptr2TreeOrData.ptr2Tree
        .resize(cursor->ptr2TreeOrData.ptr2Tree.size()-1);
    // If there is No underflow. Phew!!
    if (cursor->keys.size() >= (getMaxIntChildLimit() + 1) / 2 - 1) {
        cout << "Deleted " << x << " from internal node successfully\n";
        return;
    }
    cout << "UnderFlow in internal Node! What did you do :/" << endl;
    if (cursor == root) {
        return;
    }
    Node** p1 = findParent(root, cursor);
    Node* parent = *p1;
    int leftSibling, rightSibling;
    // Finding Left and Right Siblings as we did earlier
    for (pos = 0; pos < parent->ptr2TreeOrData.ptr2Tree.size(); pos++) {
        if (parent->ptr2TreeOrData.ptr2Tree[pos] == cursor) {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }
    // If possible transfer to leftSibling
    if (leftSibling >= 0) {
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
        //Check if LeftSibling has extra Key to transfer
        if (leftNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2 ) {
            //transfer key from left sibling through parent
            int maxIdxKey = leftNode->keys.size() - 1;
            cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
            parent->keys[leftSibling] = leftNode->keys[maxIdxKey];
            int maxIdxPtr = leftNode->ptr2TreeOrData.ptr2Tree.size()-1;
            cursor->ptr2TreeOrData.ptr2Tree
                .insert(cursor->ptr2TreeOrData.ptr2Tree.begin(), leftNode->ptr2TreeOrData.ptr2Tree[maxIdxPtr]);
            //resize the left Sibling Node After Tranfer
            leftNode->keys.resize(maxIdxKey);
            leftNode->ptr2TreeOrData.dataPtr.resize(maxIdxPtr);
            return;
        }
    }
    // If possible transfer to rightSibling
    if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
        //Check if LeftSibling has extra Key to transfer
        if (rightNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2) {
            //transfer key from right sibling through parent
            int maxIdxKey = rightNode->keys.size() - 1;
            cursor->keys.push_back(parent->keys[pos]);
            parent->keys[pos] = rightNode->keys[0];
            rightNode->keys.erase(rightNode->keys.begin());
            //transfer the pointer from rightSibling to cursor
            cursor->ptr2TreeOrData.ptr2Tree
                .push_back(rightNode->ptr2TreeOrData.ptr2Tree[0]);
            cursor->ptr2TreeOrData.ptr2Tree
                .erase(cursor->ptr2TreeOrData.ptr2Tree.begin());
            
            return;
        }
    }
    //Start to Merge Now, if None of the above cases applied
    if (leftSibling >= 0) {
        //leftNode + parent key + cursor
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
        leftNode->keys.push_back(parent->keys[leftSibling]);
        for (int val : cursor->keys) {
            leftNode->keys.push_back(val);
        }
        for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
            leftNode->ptr2TreeOrData.ptr2Tree
                .push_back(cursor->ptr2TreeOrData.ptr2Tree[i]);
            cursor->ptr2TreeOrData.ptr2Tree[i] = NULL;
        }
        cursor->ptr2TreeOrData.ptr2Tree.resize(0);
        cursor->keys.resize(0);
        removeInternal(parent->keys[leftSibling], parent, cursor);
        cout << "Merged with left sibling"<<endl;
    }
    else if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        //cursor + parentkey +rightNode
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
        cursor->keys.push_back(parent->keys[rightSibling - 1]);
        for (int val : rightNode->keys) {
            cursor->keys.push_back(val);
        }
        for (int i = 0; i < rightNode->ptr2TreeOrData.ptr2Tree.size(); i++) {
            cursor->ptr2TreeOrData.ptr2Tree
                .push_back(rightNode->ptr2TreeOrData.ptr2Tree[i]);
            rightNode->ptr2TreeOrData.ptr2Tree[i] = NULL;
        }
        rightNode->ptr2TreeOrData.ptr2Tree.resize(0);
        rightNode->keys.resize(0);
        removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
        cout << "Merged with right sibling\n";
    }
}

std::pair<int, int> BPTree::getUbicacionRegistro(int clave) {
    Node* cursor = this->root;

    // Recorrer hasta encontrar un nodo hoja
    while (!cursor->isLeaf) {
        int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), clave) - cursor->keys.begin();
        cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
    }

    // Buscar la clave dentro del nodo hoja
    int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), clave) - cursor->keys.begin();

    if (idx == cursor->keys.size() || cursor->keys[idx] != clave) {
        std::cout << "No se encontró la clave: " << clave << std::endl;
        return std::make_pair(-1, -1); // Indicar que no se encontró
    }

    return cursor->ptr2TreeOrData.dataPtr[idx];
}


void BPTree::serialize(const std::string& filename){
    std::string ruta = RUTA_BASE + filename; 
    std::ofstream out(ruta);
    if (!out) {
        std::cerr << "Error opening file for serialization: " << filename << std::endl;
        return;
    }
    if (root) {
        root->serialize(out);
    }
    out.close();
}
void BPTree::deserialize(const std::string& filename){
    std::string ruta = RUTA_BASE + filename; 
    std::ifstream in(ruta);
    if (!in) {
        std::cerr << "Error opening file for deserialization: " << filename << std::endl;
        return;
    }
    root = Node::deserialize(in);
    in.close();
}
void BPTree::toDot(const std::string& filename){
    std::string ruta = RUTA_BASE + filename;
    std::ofstream out(ruta);
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
void BPTree::rangeSearch(int startKey, int endKey) {
    if (root == NULL) {
        std::cout << "El árbol B+ está vacío." << std::endl;
        return;
    }
    
    // Encuentra el nodo hoja donde comienza el rango
    Node* cursor = root;
    while (!cursor->isLeaf) {
        int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), startKey) - cursor->keys.begin();
        cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
    }

    // Recorre los nodos hoja a partir del nodo encontrado
    while (cursor != NULL) {
        for (size_t i = 0; i < cursor->keys.size(); ++i) {
            if (cursor->keys[i] >= startKey && cursor->keys[i] <= endKey) {
                std::cout << "Clave: " << cursor->keys[i] << std::endl;
                // Aquí puedes hacer lo que necesites con los datos
                // Por ejemplo, puedes imprimir los valores asociados a la clave
            }
        }
        if (cursor->keys.back() > endKey) {
            // Si la última clave en el nodo hoja es mayor que el final del rango, no necesitamos buscar más
            break;
        }
        cursor = cursor->ptr2next; // Mover al siguiente nodo hoja
    }
}
