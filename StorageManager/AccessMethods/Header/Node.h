#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class Node {
   public:
    bool isLeaf;
    vector<int> keys;
    Node* ptr2next;
    std::tuple<int, int> ubicacion;            //Pointer to connect next node for leaf nodes
    union ptr {                  //to make memory efficient Node
        vector<Node*> ptr2Tree;  //Array of pointers to Children sub-trees for intermediate Nodes
        vector<FILE*> dataPtr;   // Data-Pointer for the leaf node

        ptr(){}   // To remove the error !?
        ~ptr(){}  // To remove the error !?
    } ptr2TreeOrData;
    friend class BPTree;  // to access private members of the Node and hold the encapsulation concept
   
   public:
    Node();
    void serialize(std::ofstream& out);
    static Node* deserialize(std::ifstream& in);
    void toDot(std::ofstream& out, int& nodeCount);
};
