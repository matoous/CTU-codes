#ifndef PDV_HW03_BST_H
#define PDV_HW03_BST_H


#include <atomic>

class bst_tree {
public:

    class node {
    public:
        std::atomic<node *> left {nullptr};
        std::atomic<node *> right {nullptr};

        long long data;

        node(long long data) : data(data) {}
    };

    std::atomic<node *>  root {nullptr};

    ~bst_tree();
    void insert(long long data);
};


#endif //PDV_HW03_BST_H
