#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include <limits>
#include <sstream>
#include <queue>

using namespace std;

class Node {
   public:
    bool isLeaf;
    vector<int> keys;
    Node* ptr2next;              //Pointer to connect next node for leaf nodes
    union ptr {                  //to make memory efficient Node
        vector<Node*> ptr2Tree;  //Array of pointers to Children sub-trees for intermediate Nodes
        vector<FILE*> dataPtr;   // Data-Pointer for the leaf node

        ptr(){}   // To remove the error !?
        ~ptr(){}  // To remove the error !?
    } ptr2TreeOrData;
    friend class BPTree;  // to access private members of the Node and hold the encapsulation concept
   
   public:
    Node(){
        this->isLeaf = false;
        this->ptr2next = NULL;
    }
    
    void serialize(std::ofstream& out){
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

    void deleteInSerialize(std::ofstream& out) {
        if (isLeaf) {
            // Escribir información de nodos hoja
            out << "Leaf cant: " << keys.size() << " |\n";
            for (size_t i = 0; i < keys.size(); ++i) {
                out << "Leaf_place " << keys[i] << " path\n";  // "path" es un placeholder
            }
        } else {
            // Escribir información de nodos internos
            out << "Internal cant: " << keys.size() << " |";
            for (int key : keys) {
                out << " " << key;
            }
            out << " cant_hijos: " << ptr2TreeOrData.ptr2Tree.size() << "\n";
            for (Node* child : ptr2TreeOrData.ptr2Tree) {
                if (child) {
                    child->deleteInSerialize(out);
                }
            }
        }
    }

    static Node* deserialize(std::ifstream& in){
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

    void toDot(std::ofstream& out, int& nodeCount) {
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

};

Node* parent = NULL;

class BPTree {
   private:
    int maxIntChildLimit;                                   //Limiting  #of children for internal Nodes!
    int maxLeafNodeLimit;                                   // Limiting #of nodes for leaf Nodes!!!
    Node* root;                                             //Pointer to the B+ Tree root
    void insertInternal(int x, Node** cursor, Node** child){  // in Internal Nodes
        if ((*cursor)->keys.size() < maxIntChildLimit - 1) {
            /*
                If cursor is not full find the position for the new key.
            */
            int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
            (*cursor)->keys.push_back(x);
            // new (&(*cursor)->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
            //// now, root->ptr2TreeOrData.ptr2Tree is the active member of the union
            (*cursor)->ptr2TreeOrData.ptr2Tree.push_back(*child);

            if (i != (*cursor)->keys.size() - 1) {  // if there are more than one element
                // Different loops because size is different for both (i.e. diff of one)

                for (int j = (*cursor)->keys.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                    (*cursor)->keys[j] = (*cursor)->keys[j - 1];
                }

                for (int j = (*cursor)->ptr2TreeOrData.ptr2Tree.size() - 1; j > (i + 1); j--) {
                    (*cursor)->ptr2TreeOrData.ptr2Tree[j] = (*cursor)->ptr2TreeOrData.ptr2Tree[j - 1];
                }

                (*cursor)->keys[i] = x;
                (*cursor)->ptr2TreeOrData.ptr2Tree[i + 1] = *child;
            }
            cout << "Insertado optimamente el key AAinternal" << endl;
        } else {
            vector<int> virtualKeyNode((*cursor)->keys);
            vector<Node*> virtualTreePtrNode((*cursor)->ptr2TreeOrData.ptr2Tree);

            int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();  // finding the position for x
            virtualKeyNode.push_back(x);                                                                    // to create space
            virtualTreePtrNode.push_back(*child);                                                            // to create space

            if (i != virtualKeyNode.size() - 1) {
                for (int j = virtualKeyNode.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                    virtualKeyNode[j] = virtualKeyNode[j - 1];
                }

                for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--) {
                    virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
                }

                virtualKeyNode[i] = x;
                virtualTreePtrNode[i + 1] = *child;
            }

            int partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2) - 1];  // exclude middle element while splitting
            int partitionIdx = (virtualKeyNode.size() / 2) - 1;
            cout << partitionKey << partitionIdx << endl;

            // resizing and copying the keys & TreePtr to OldNode
            (*cursor)->keys.resize(partitionIdx);
            (*cursor)->ptr2TreeOrData.ptr2Tree.resize(partitionIdx + 1);
            for (int i = 0; i < partitionIdx; i++) {
                (*cursor)->keys[i] = virtualKeyNode[i];
            }

            for (int i = 0; i < partitionIdx + 1; i++) {
                (*cursor)->ptr2TreeOrData.ptr2Tree[i] = virtualTreePtrNode[i];
            }

            Node* newInternalNode = new Node;
            new (&newInternalNode->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
            // Pushing new keys & TreePtr to NewNode

            for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
                newInternalNode->keys.push_back(virtualKeyNode[i]);
            }

            for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {  // because only key is excluded not the pointer
                newInternalNode->ptr2TreeOrData.ptr2Tree.push_back(virtualTreePtrNode[i]);
            }

            if ((*cursor) == root) {
                /*
                    If cursor is a root we create a new Node
                */
                Node* newRoot = new Node;
                newRoot->keys.push_back(partitionKey);
                new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(*cursor);
                //// now, newRoot->ptr2TreeOrData.ptr2Tree is the active member of the union
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternalNode);

                root = newRoot;
                cout << "Se creo una nueva raiz!" << endl;
            } else {
                /*
                    ::Recursion::
                */
                insertInternal(partitionKey, findParent(root, *cursor), &newInternalNode);
            }
        }
    }
    Node** findParent(Node* cursor, Node* child){
        /*
            Finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
            also ignores second last level because we will never find parent of a leaf node during insertion using this function
        */

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
    Node* firstLeftNode(Node* cursor){
        if (cursor->isLeaf)
            return cursor;
        for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
            if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL)
                return firstLeftNode(cursor->ptr2TreeOrData.ptr2Tree[i]);

        return NULL;
    }

   public:
    BPTree(){
        this->maxIntChildLimit = 4;
        this->maxLeafNodeLimit = 3;
        this->root = NULL;
    }
    BPTree(int degreeInternal, int degreeLeaf){
        this->maxIntChildLimit = degreeInternal;
        this->maxLeafNodeLimit = degreeLeaf;
        this->root = NULL;
    }
    Node* getRoot(){
        return this->root;
    }
    int getMaxIntChildLimit(){
        return maxIntChildLimit;
    }
    int getMaxLeafNodeLimit(){
        return maxLeafNodeLimit;
    }
    void setRoot(Node *ptr) {
        this->root = ptr;
    }
    void display(Node* cursor){
        if (cursor == NULL) return;
        queue<Node*> q;
        q.push(cursor);

        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                Node* u = q.front(); q.pop();

                //printing keys in self
                for (int val : u->keys)
                    cout << val << " ";

                cout << "|| ";//to seperate next adjacent nodes
                
                if (u->isLeaf != true) {
                    for (Node* v : u->ptr2TreeOrData.ptr2Tree) {
                        q.push(v);
                    }
                }
            }
            cout << endl;
        }
    }
    void seqDisplay(Node* cursor){
        Node* firstLeft = firstLeftNode(cursor);

        if (firstLeft == NULL) {
            cout << "No hay datos, arbol vacio ...!" << endl;
            return;
        }
        while (firstLeft != NULL) {
            for (int i = 0; i < firstLeft->keys.size(); i++) {
                cout << firstLeft->keys[i] << " ";
            }

            firstLeft = firstLeft->ptr2next;
        }
        cout << endl;
    }
    void search(int key) {
        if (root == NULL) {
            cout << "NO Tuples Inserted yet" << endl;
            return;
        } else {
            Node* cursor = root;
            while (cursor->isLeaf == false) {

                int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];  //upper_bound takes care of all the edge cases
            }

            int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();  //Binary search

            if (idx == cursor->keys.size() || cursor->keys[idx] != key) {
                cout << "No se encontro ese key" << endl;
                return;
            }

            /*
            Podemos recuperar los datos del disco en la memoria principal usando data-ptr
            usando cursor->dataPtr[idx]
            */

            string fileName = "DBFiles/";
            string data;
            fileName += to_string(key) + ".txt";
            FILE* filePtr = fopen(fileName.c_str(), "r");
            cout << "Se encontro el Key" << endl;
            cout << "Datos que corresponden a ese id: ";
            char ch = fgetc(filePtr);
            while (ch != EOF) {
                printf("%c", ch);
                ch = fgetc(filePtr);
            }
            fclose(filePtr);
            cout << endl;
        }
    }
    void insert(int key, FILE* filePtr) {  // in Leaf Node
        if (root == NULL) {
            root = new Node;
            root->isLeaf = true;
            root->keys.push_back(key);
            new (&root->ptr2TreeOrData.dataPtr) std::vector<FILE*>;
            root->ptr2TreeOrData.dataPtr.push_back(filePtr);

            cout << key << " es la raiz!!" << endl;
            return;
        } else {
            Node* cursor = root;
            Node* parent = NULL;
            // searching for the possible position for the given key by doing the same procedure we did in search
            while (cursor->isLeaf == false) {
                parent = cursor;
                int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
            }

            // now cursor is the leaf node in which we'll insert the new key
            if (cursor->keys.size() < maxLeafNodeLimit) {
                /*
                    If current leaf Node is not FULL, find the correct position for the new key and insert!
                */
                int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                cursor->keys.push_back(key);
                cursor->ptr2TreeOrData.dataPtr.push_back(filePtr);

                if (i != cursor->keys.size() - 1) {
                    for (int j = cursor->keys.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                        cursor->keys[j] = cursor->keys[j - 1];
                        cursor->ptr2TreeOrData.dataPtr[j] = cursor->ptr2TreeOrData.dataPtr[j - 1];
                    }

                    // since earlier step was just to inc. the size of vectors and making space, now we are simply inserting
                    cursor->keys[i] = key;
                    cursor->ptr2TreeOrData.dataPtr[i] = filePtr;
                }
                cout << "Insertado optimamente el key: " << key << endl;
            } else {
                /*
                    DAMN!! Node Overflowed :(
                    HAIYYA! Splitting the Node .
                */
                vector<int> virtualNode(cursor->keys);
                vector<FILE*> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);

                // finding the probable place to insert the key
                int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

                virtualNode.push_back(key);          // to create space
                virtualDataNode.push_back(filePtr);  // to create space

                if (i != virtualNode.size() - 1) {
                    for (int j = virtualNode.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                        virtualNode[j] = virtualNode[j - 1];
                        virtualDataNode[j] = virtualDataNode[j - 1];
                    }

                    // inserting
                    virtualNode[i] = key;
                    virtualDataNode[i] = filePtr;
                }
                /*
                    BAZINGA! I have the power to create new Leaf :)
                */

                Node* newLeaf = new Node;
                newLeaf->isLeaf = true;
                new (&newLeaf->ptr2TreeOrData.dataPtr) std::vector<FILE*>;
                //// now, newLeaf->ptr2TreeOrData.ptr2Tree is the active member of the union

                // swapping the next ptr
                Node* temp = cursor->ptr2next;
                cursor->ptr2next = newLeaf;
                newLeaf->ptr2next = temp;

                // resizing and copying the keys & dataPtr to OldNode
                int partitionIdx = maxLeafNodeLimit / 2;
                cursor->keys.resize(partitionIdx);
                cursor->ptr2TreeOrData.dataPtr.resize(partitionIdx);
                for (int i = 0; i < partitionIdx; i++) {
                    cursor->keys[i] = virtualNode[i];
                    cursor->ptr2TreeOrData.dataPtr[i] = virtualDataNode[i];
                }

                // Pushing new keys & dataPtr to NewNode
                for (int i = partitionIdx; i < virtualNode.size(); i++) {
                    newLeaf->keys.push_back(virtualNode[i]);
                    newLeaf->ptr2TreeOrData.dataPtr.push_back(virtualDataNode[i]);
                }

                if (cursor == root) {
                    /*
                        If cursor is root node we create new node
                    */

                    Node* newRoot = new Node;
                    newRoot->keys.push_back(newLeaf->keys[0]);
                    new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
                    newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
                    newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
                    root = newRoot;
                    cout << "Se modifico la raiz!" << endl;
                } else {
                    // Insert new key in the parent
                    insertInternal(newLeaf->keys[0], &parent, &newLeaf);
                }
            }
        }
    }
    void removeKey(int x){
        Node* root = getRoot();

        // If tree is empty
        if (root == NULL) {
            cout << "B+ Tree is Empty" << endl;
            return;
        }

        Node* cursor = root;
        Node* parent;
        int leftSibling, rightSibling;

        // Going to the Leaf Node, Which may contain the *key*
        // TO-DO : Use Binary Search to find the val
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

        // Check if the value exists in this leaf node
        int pos = 0;
        bool found = false;
        for (pos = 0; pos < cursor->keys.size(); pos++) {
            if (cursor->keys[pos] == x) {
                found = true;
                break;
            }
        }

        auto itr = lower_bound(cursor->keys.begin(), cursor->keys.end(), x);

        if (found == false) {
            cout << "Key Not Found in the Tree" << endl;
            return;
        }
        
        // Delete the respective File and FILE*
        string fileName = "DBFiles/" + to_string(x) + ".txt";
        char filePtr[256];
        strcpy(filePtr, fileName.c_str());

        //delete cursor->ptr2TreeOrData.dataPtr[pos];//avoid memory leaks
        if (remove(filePtr) == 0)
            cout << "SuccessFully Deleted file: " << fileName << endl;
        else
            cout << "Unable to delete the file: " << fileName << endl;

        // Shifting the keys and dataPtr for the leaf Node
        for (int i = pos; i < cursor->keys.size()-1; i++) {
            cursor->keys[i] = cursor->keys[i+1];
            cursor->ptr2TreeOrData.dataPtr[i] = cursor->ptr2TreeOrData.dataPtr[i + 1];
        }
        int prev_size = cursor->keys.size();
        cursor->keys.resize(prev_size - 1);
        cursor->ptr2TreeOrData.dataPtr.resize(prev_size - 1);

        // If it is leaf as well as the root node
        if (cursor == root) {
            if (cursor->keys.size() == 0) {
                // Tree becomes empty
                setRoot(NULL);
                cout << "Ohh!! Our Tree is Empty Now :(" << endl;
            }
        }
        
        cout << "Deleted " << x << " From Leaf Node successfully" << endl;
        if ((cursor->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2) || (cursor == root)) {
            //Sufficient Node available for invariant to hold
            return;
        }

        cout << "UnderFlow in the leaf Node Happended" << endl;
        cout << "Starting Redistribution..." << endl;

        //1. Try to borrow a key from leftSibling
        if (leftSibling >= 0) {
            Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

            //Check if LeftSibling has extra Key to transfer
            if (leftNode->keys.size() >= (getMaxLeafNodeLimit()+1) / 2 +1) {

                //Transfer the maximum key from the left Sibling
                int maxIdx = leftNode->keys.size()-1;
                cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
                cursor->ptr2TreeOrData.dataPtr
                    .insert(cursor->ptr2TreeOrData.dataPtr.begin(), leftNode->ptr2TreeOrData.dataPtr[maxIdx]);

                //resize the left Sibling Node After Tranfer
                leftNode->keys.resize(maxIdx);
                leftNode->ptr2TreeOrData.dataPtr.resize(maxIdx);

                //Update Parent
                parent->keys[leftSibling] = cursor->keys[0];
                printf("Transferred from left sibling of leaf node\n");
                return;
            }
        }

        //2. Try to borrow a key from rightSibling
        if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
            Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

            //Check if RightSibling has extra Key to transfer
            if (rightNode->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2 + 1) {

                //Transfer the minimum key from the right Sibling
                int minIdx = 0;
                cursor->keys.push_back(rightNode->keys[minIdx]);
                cursor->ptr2TreeOrData.dataPtr
                    .push_back(rightNode->ptr2TreeOrData.dataPtr[minIdx]);

                //resize the right Sibling Node After Tranfer
                rightNode->keys.erase(rightNode->keys.begin());
                rightNode->ptr2TreeOrData.dataPtr.erase(rightNode->ptr2TreeOrData.dataPtr.begin());

                //Update Parent
                parent->keys[rightSibling-1] = rightNode->keys[0];
                printf("Transferred from right sibling of leaf node\n");
                return;
            }
        }

        // Merge and Delete Node
        if (leftSibling >= 0) {// If left sibling exists
            Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
            //Transfer Key and dataPtr to leftSibling and connect ptr2next
            for (int i = 0; i < cursor->keys.size(); i++) {
                leftNode->keys.push_back(cursor->keys[i]);
                leftNode->ptr2TreeOrData.dataPtr
                    .push_back(cursor->ptr2TreeOrData.dataPtr[i]);
            }
            leftNode->ptr2next = cursor->ptr2next;
            cout << "Merging two leaf Nodes" << endl;
            removeInternal(parent->keys[leftSibling], parent, cursor);//delete parent Node Key
            //delete cursor;
        }
        else if (rightSibling <= parent->keys.size()) {
            Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
            //Transfer Key and dataPtr to rightSibling and connect ptr2next
            for (int i = 0; i < rightNode->keys.size(); i++) {
                cursor->keys.push_back(rightNode->keys[i]);
                cursor->ptr2TreeOrData.dataPtr
                    .push_back(rightNode->ptr2TreeOrData.dataPtr[i]);
            }
            cursor->ptr2next = rightNode->ptr2next;
            cout << "Merging two leaf Nodes" << endl;
            removeInternal(parent->keys[rightSibling-1], parent, rightNode);//delete parent Node Key
            //delete rightNode;
        }

    }
    void removeInternal(int x, Node* cursor, Node* child){
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

    void serialize(const std::string& filename){
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
    void deserialize(const std::string& filename){
        std::ifstream in(filename);
        if (!in) {
            std::cerr << "Error opening file for deserialization: " << filename << std::endl;
            return;
        }
        root = Node::deserialize(in);
        in.close();
    }

    void updateSerialization(const std::string& filename) {
        std::ofstream outFile(filename, std::ios::trunc);
        if (outFile.is_open()) {
            if (root) {
                root->deleteInSerialize(outFile);
            }
            outFile.close();
        } else {
            std::cerr << "Error: No se pudo abrir el archivo para escritura." << std::endl;
        }
    }

    void toDot(const std::string& filename){
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
};

void insertionMethod(BPTree** bPTree) {
    int postid, userid, hour;
    string text;
    //indice denso por postid

    cout << "\nIndica Postid: ";
    cin >> postid; // >> userid >> hour >> text;

    string fileName = "DBFiles/";

    //nombre del archivo con el cual se encuentra
    fileName += to_string(postid) + ".txt";
    FILE* filePtr = fopen(fileName.c_str(), "w");
    string userTuple = to_string(postid) + "|"
                        + to_string(userid) + "|" 
                        + to_string(hour) + "|" 
                        + text +  "\n";
    fprintf(filePtr, "%s", userTuple.c_str());

    //indice denso por postid
    (*bPTree)->insert(postid, filePtr);
    fclose(filePtr);
    cout << "Se inserto el postid:" << postid <<endl;

    //LLENAR EL ARBOL
    (*bPTree)->serialize("bptree.dat");
}

void searchMethod(BPTree* bPTree) {
    int idx;
    cout << "Cual es el postid? ";
    cin >> idx;

    bPTree->search(idx);
}

void printMethod(BPTree* bPTree) {
    int opt;
    cout << "\n\t1.Arbol de niveles \n\t2. Forma secuencial\n";
    cin >> opt;

    cout << endl;
    if (opt == 1)
        bPTree->display(bPTree->getRoot());
    else
        bPTree->seqDisplay(bPTree->getRoot());
}

void deleteMethod(BPTree* bPTree) {
    cout << "Del arbol, cual deseas borrar?: " << endl;
    bPTree->display(bPTree->getRoot());
 
    int tmp;
    cout << "Clave a borrar: " << endl;
    cin >> tmp;
    bPTree->removeKey(tmp);

    cout << "Arbol actualizado" << endl;
    bPTree->display(bPTree->getRoot());

    //Borrar en el ARBOL
    (bPTree)->updateSerialization("bptree.dat");
}

int main() {
    cout << "\n************* Menu B+ Tree *************\n";

    bool flag = true;
    int option;
    int degree;

    cout << "Grado del arbol: ";
    cin >> degree;
    degree = degree*2;

    BPTree* bPTree = new BPTree(degree, degree);

    do {
        
        cout << "0: Recuperar arbol \n1: Insertar \n2: Buscar \n3: Imprimir arbol\n4: Borrar\n5: Generar imagen del arbol\n6: Salir" << endl;
        cout << "\tElija una opcion : ";
        cin >> option;

        switch (option) {
            case 0:
                // Recuperar el arbol
                std::cout << "Recuperando el árbol..." << std::endl;

                bPTree->deserialize("bptree.dat");

                if (bPTree->getRoot() == nullptr) {
                    std::cerr << "Error: No se pudo recuperar el árbol." << std::endl;
                } else {
                    std::cout << "Árbol recuperado exitosamente." << std::endl;
                }

                break;
            case 1:
                // Inserta valores al arbol
                insertionMethod(&bPTree);
                break;
            case 2:
                // Busca valores al arbol
                searchMethod(bPTree);
                break;
            case 3:
                // Imprime el arbol
                printMethod(bPTree);
                break;
            case 4:
                // Borra valores del arbol
                deleteMethod(bPTree);
                break;
            case 5:
                //Genera imagen del arbol
                bPTree->toDot("bptree.dot");
                std::cout << "El archivo DOT se ha generado como 'bptree.dot'. Usa Graphviz para visualizarlo." << std::endl;
                break;
            default:
                flag = false;
                break;
        }
    }while (flag);


    return 0;
}
