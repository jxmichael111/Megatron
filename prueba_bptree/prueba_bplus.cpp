#include <iostream>
#include <vector>
#include <memory>
#include <algorithm> // Necesario para std::sort y std::find
#include <queue>
class Node {
public:
    bool isLeaf;
    std::vector<int> keys;
    std::vector<std::shared_ptr<Node>> ptr2Tree; // Para nodos internos
    std::vector<int> dataPtr; // Para nodos hoja
    std::shared_ptr<Node> ptr2next; // Apuntador al siguiente nodo hoja

    Node(bool leaf = true) : isLeaf(leaf), ptr2next(nullptr) {}
};

class BPTree {
    int maxInternalNodeLimit, maxLeafNodeLimit;
    std::shared_ptr<Node> root;

public:
    BPTree(int _maxInternalNodeLimit, int _maxLeafNodeLimit)
        : maxInternalNodeLimit(_maxInternalNodeLimit), maxLeafNodeLimit(_maxLeafNodeLimit), root(nullptr) {}

    std::shared_ptr<Node> getRoot() {
        return root;
    }

    void insert(int x) {
        if (!root) {
            root = std::make_shared<Node>();
            root->isLeaf = true;
            root->keys.push_back(x);
            return;
        }
        
        std::shared_ptr<Node> cursor = root;
        std::shared_ptr<Node> parent;

        while (!cursor->isLeaf) {
            parent = cursor;
            for (int i = 0; i < cursor->keys.size(); i++) {
                if (x < cursor->keys[i]) {
                    cursor = cursor->ptr2Tree[i];
                    break;
                }
                if (i == cursor->keys.size() - 1) {
                    cursor = cursor->ptr2Tree[i + 1];
                    break;
                }
            }
        }

        if (cursor->keys.size() < maxLeafNodeLimit) {
            auto it = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), x);
            cursor->keys.insert(it, x);
        } else {
            auto newLeaf = std::make_shared<Node>();
            newLeaf->isLeaf = true;

            std::vector<int> virtualNode(cursor->keys);
            virtualNode.push_back(x);
            std::sort(virtualNode.begin(), virtualNode.end());

            int partitionIdx = (maxLeafNodeLimit + 1) / 2;
            cursor->keys.resize(partitionIdx);
            cursor->dataPtr.resize(partitionIdx);

            for (int i = 0; i < partitionIdx; i++) {
                cursor->keys[i] = virtualNode[i];
            }

            for (int i = partitionIdx; i < virtualNode.size(); i++) {
                newLeaf->keys.push_back(virtualNode[i]);
            }

            newLeaf->ptr2next = cursor->ptr2next;
            cursor->ptr2next = newLeaf;

            if (cursor == root) {
                auto newRoot = std::make_shared<Node>();
                newRoot->isLeaf = false;
                newRoot->keys.push_back(newLeaf->keys[0]);
                newRoot->ptr2Tree.push_back(cursor);
                newRoot->ptr2Tree.push_back(newLeaf);
                root = newRoot;
            } else {
                insertInternal(newLeaf->keys[0], parent, newLeaf);
            }
        }
    }

    void insertInternal(int x, std::shared_ptr<Node> cursor, std::shared_ptr<Node> child) {
        if (cursor->keys.size() < maxInternalNodeLimit) {
            auto it = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), x);
            cursor->keys.insert(it, x);
            cursor->ptr2Tree.insert(cursor->ptr2Tree.begin() + (it - cursor->keys.begin() + 1), child);
        } else {
            auto newInternal = std::make_shared<Node>();
            newInternal->isLeaf = false;

            std::vector<int> virtualNode(cursor->keys);
            virtualNode.push_back(x);
            std::sort(virtualNode.begin(), virtualNode.end());

            int partitionIdx = (maxInternalNodeLimit + 1) / 2;
            cursor->keys.resize(partitionIdx);

            for (int i = 0; i < partitionIdx; i++) {
                cursor->keys[i] = virtualNode[i];
            }

            for (int i = partitionIdx + 1; i < virtualNode.size(); i++) {
                newInternal->keys.push_back(virtualNode[i]);
            }

            if (cursor == root) {
                auto newRoot = std::make_shared<Node>();
                newRoot->isLeaf = false;
                newRoot->keys.push_back(cursor->keys[partitionIdx]);
                newRoot->ptr2Tree.push_back(cursor);
                newRoot->ptr2Tree.push_back(newInternal);
                root = newRoot;
            } else {
                insertInternal(cursor->keys[partitionIdx], findParent(root, cursor), newInternal);
            }
        }
    }

    std::shared_ptr<Node> findParent(std::shared_ptr<Node> cursor, std::shared_ptr<Node> child) {
        if (cursor->isLeaf || cursor->ptr2Tree[0]->isLeaf) {
            return nullptr;
        }

        for (int i = 0; i < cursor->ptr2Tree.size(); i++) {
            if (cursor->ptr2Tree[i] == child) {
                return cursor;
            } else {
                auto parent = findParent(cursor->ptr2Tree[i], child);
                if (parent) return parent;
            }
        }

        return nullptr;
    }

    void display(std::shared_ptr<Node> cursor) {
        if (!cursor) return;

        std::queue<std::shared_ptr<Node>> q;
        q.push(cursor);

        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                std::shared_ptr<Node> u = q.front(); q.pop();

                for (int val : u->keys)
                    std::cout << val << " ";

                std::cout << "|| ";

                if (!u->isLeaf) {
                    for (auto& v : u->ptr2Tree) {
                        q.push(v);
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    void removeKey(int x) {
        if (!root) {
            std::cout << "B+ Tree is Empty";
            return;
        }
        
        std::shared_ptr<Node> cursor = root;
        std::shared_ptr<Node> parent;

        while (!cursor->isLeaf) {
            parent = cursor;
            for (int i = 0; i < cursor->keys.size(); i++) {
                if (x < cursor->keys[i]) {
                    cursor = cursor->ptr2Tree[i];
                    break;
                }
                if (i == cursor->keys.size() - 1) {
                    cursor = cursor->ptr2Tree[i + 1];
                    break;
                }
            }
        }

        auto it = std::find(cursor->keys.begin(), cursor->keys.end(), x);
        if (it != cursor->keys.end()) {
            cursor->keys.erase(it);
            std::cout << "Key " << x << " removed.";
        } else {
            std::cout << "Key " << x << " not found.";
        }
    }
};

int main() {
    std::cout << "\n************* Menu B+ Tree *************\n";

    bool flag = true;
    int option;
    int degree;

    std::cout << "Grado del arbol: ";
    std::cin >> degree;
    degree = degree * 2;

    auto bPTree = std::make_shared<BPTree>(degree, degree);

    do {
        std::cout << "0: Recuperar arbol \n1: Insertar \n2: Buscar \n3: Imprimir arbol\n4: Borrar\n5: Generar imagen del arbol\n6: Salir" << std::endl;
        std::cout << "\tElija una opcion : ";
        std::cin >> option;

        switch (option) {
            case 0:
                std::cout << "Función de recuperación aún no implementada.\n";
                break;
            case 1:
                // Inserta valores al arbol
                std::cout << "Ingrese el valor a insertar: ";
                int val;
                std::cin >> val;
                bPTree->insert(val);
                break;
            case 2:
                std::cout << "Función de búsqueda aún no implementada.\n";
                break;
            case 3:
                // Imprime el arbol
                bPTree->display(bPTree->getRoot());
                break;
            case 4:
                // Borra valores del arbol
                std::cout << "Ingrese el valor a borrar: ";
                std::cin >> val;
                bPTree->removeKey(val);
                break;
            case 5:
                std::cout << "Función de generación de imagen aún no implementada.\n";
                break;
            default:
                flag = false;
                break;
        }
    } while (flag);

    return 0;
}
