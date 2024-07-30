/*
#include <iostream>
#include <queue>
#ifndef NODE_H
#include "B+ Tree.h"


void BPTree::display(Node* cursor) {
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

void BPTree::seqDisplay(Node* cursor) {
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

#endif 
*/