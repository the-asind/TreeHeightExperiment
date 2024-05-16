#include <utility>
#include <iostream>
#include <tuple>  // Add this line


#include "RBST.cpp"



struct Sum {
    using T = int;
    using E = int;

    static T dt() { return 0; }
    static E de() { return 0; }
    static T f(T a, T b, T c) { return a + b + c; }
    static T g(T a, E b, int len) { return a + b * len; }
    static E h(E a, E b) { return a + b; }
};

void test1(){
    RBST<Sum> tree;
    RBST<Sum>::node* root = nullptr;

    // Insert elements
    for (int i = 1; i <= 10; ++i) {
        tree.insert(root, i - 1, i);
    }

    // Print the tree
    std::cout << "Tree: ";
    tree.debug(root);
    std::cout << std::endl;

    // Query sum of elements in range [2, 7)
    std::cout << "Sum of elements in range [2, 7): " << tree.query(root, 2, 7) << std::endl;

    // Update elements in range [3, 6) with value 2
    tree.update(root, 3, 6, 2);

    // Print the updated tree
    std::cout << "Updated tree: ";
    tree.debug(root);
    std::cout << std::endl;

    // Query sum of elements in range [2, 7) after update
    std::cout << "Sum of elements in range [2, 7) after update: " << tree.query(root, 2, 7) << std::endl;

    // Erase element at position 4
    tree.erase(root, 4);

    // Print the tree after erasure
    std::cout << "Tree after erasure: ";
    tree.debug(root);
    std::cout << std::endl;
}

void test2(){
    RBST<Sum> tree;
    RBST<Sum>::node* root = nullptr;

    // Insert elements
    for (int i = 1; i <= 4; ++i) {
        tree.insert(root, i - 1, i);
    }

    // Print the tree
    std::cout << "Tree: ";
    tree.debug(root);
    std::cout << std::endl;
    tree.update(root, 1, 4, 2);
    // Print the updated tree
    std::cout << "Updated tree: ";
    tree.debug(root);
    std::cout << std::endl;
}

int main() {
    test2();
    return 0;
}