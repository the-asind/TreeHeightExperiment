#include <iostream>
#include <cstdlib>
#include <ctime>

class TreapNode {
public:
    int key, priority;
    TreapNode *left, *right;

    TreapNode(int _key) : key(_key), priority(rand()), left(nullptr), right(nullptr) {}
};

class Treap {
private:
    TreapNode* root;

    TreapNode* rotateLeft(TreapNode* node) {
        TreapNode* rightChild = node->right;
        node->right = rightChild->left;
        rightChild->left = node;
        return rightChild;
    }

    TreapNode* rotateRight(TreapNode* node) {
        TreapNode* leftChild = node->left;
        node->left = leftChild->right;
        leftChild->right = node;
        return leftChild;
    }

    TreapNode* insert(TreapNode* node, int key) {
        if (!node) return new TreapNode(key);

        if (key < node->key) {
            node->left = insert(node->left, key);
            if (node->left->priority > node->priority)
                node = rotateRight(node);
        } else {
            node->right = insert(node->right, key);
            if (node->right->priority > node->priority)
                node = rotateLeft(node);
        }
        return node;
    }

    void inorder(TreapNode* node) {
        if (!node) return;
        inorder(node->left);
        std::cout << node->key << " ";
        inorder(node->right);
    }

public:
    Treap() : root(nullptr) {}

    void insert(int key) {
        root = insert(root, key);
    }

    void display() {
        inorder(root);
        std::cout << std::endl;
    }

    int height(TreapNode* node) {
        if (!node) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    }

    int getHeight() {
        return height(root);
    }
};

// Функция для тестирования высоты дерева от количества элементов
void testTreeHeight(int numElements) {
    Treap treap;
    srand(time(nullptr));

    for (int i = 0; i < numElements; ++i) {
        treap.insert(rand() % 100);
    }

    std::cout << "Height of Treap with " << numElements << " elements: " << treap.getHeight() << std::endl;
}

int main() {
    testTreeHeight(1);
    testTreeHeight(3);
    testTreeHeight(7);
    testTreeHeight(10); // Пример: тест с 10 элементами
    testTreeHeight(100);
    testTreeHeight(1000);
    testTreeHeight(10000);
    testTreeHeight(100000);
    return 0;
}