#pragma once
#include <vector>

using std::size_t;

/**
 * Nevyvažovaný binární vyhledávací strom
 */
class BST {
    struct node;
    node* m_root = nullptr;

public:
    /**
     * Vrátí všechny prvky obsažené v tomto stromu
     *
     * Pořadí navrácených prvků je dané preorder průchodem stromu
     */
    std::vector<double> preorder_elements() const;

    BST() = default;
    BST(BST const& rhs);
    BST& operator=(BST const& rhs);
    BST(BST&& rhs);
    BST& operator=(BST&& rhs);
    ~BST();

    void add(double value);
    bool contains(double value) const;

private:
    struct node {
        node* left = nullptr;
        node* right = nullptr;
        double value = 0;
    };
};
