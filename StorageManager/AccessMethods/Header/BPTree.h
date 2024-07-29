#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <limits>
#include <sstream>
#include <queue>

#include "Node.h"
#include "C:\Users\Michael\Documents\University\5to semestre\BDII\Megatron\StorageManager\DiskManager\Header\config.h"

class BPTree {
   private:
    int maxIntChildLimit;                                   //Limiting  #of children for internal Nodes!
    int maxLeafNodeLimit;                                   // Limiting #of nodes for leaf Nodes!!!
    Node* root;                                             //Pointer to the B+ Tree root
    void insertInternal(int x, Node** cursor, Node** child); // in Internal Nodes

    Node** findParent(Node* cursor, Node* child);
    Node* firstLeftNode(Node* cursor);

   public:
    BPTree();
    BPTree(int degreeInternal, int degreeLeaf);
    Node* getRoot();
    int getMaxIntChildLimit();
    int getMaxLeafNodeLimit();
    void setRoot(Node *ptr);
    void display(Node* cursor);
    void seqDisplay(Node* cursor);
    void search(int key);
    void insert(int key, FILE* filePtr);
    void removeKey(int x);
    void removeInternal(int x, Node* cursor, Node* child);

    void serialize(const std::string& filename);
    void deserialize(const std::string& filename);
    void toDot(const std::string& filename);
};