#include "tiny-04.hpp"

#include <stack>

std::vector<double> BST::preorder_elements() const {
    if (!m_root) {
        return {};
    }

    std::vector<double> values;
    std::stack<node*> nodes;
    nodes.push(m_root);

    while (!nodes.empty()) {
        auto current = nodes.top(); nodes.pop();

        values.push_back(current->value);
        if (current->right) {
            nodes.push(current->right);
        }
        if (current->left) {
            nodes.push(current->left);
        }
    }

    return values;
}

void BST::add(double value) {
    auto position = &m_root;
    while (*position != nullptr) {
        if (value < (*position)->value) {
            position = &(*position)->left;
        } else if (value > (*position)->value) {
            position = &(*position)->right;
        } else {
            // This value is already present...
            return;
        }
    }
    *position = new node{ nullptr, nullptr, value };
}

bool BST::contains(double value) const {
    auto next = m_root;
    while (next != nullptr) {
        if (value < next->value) {
            next = next->left;
        } else if (value > next->value) {
            next = next->right;
        } else {
            // Found the right value
            return true;
        }
    }
    // Walked the tree without finding the value
    return false;
}

BST::BST(BST const& rhs) {
    if(&rhs == this) return;
    for(auto ele : rhs.preorder_elements()){
        add(ele);
    }
};

BST& BST::operator=(BST const& rhs) {
    if(&rhs == this) return (*this);
    if (m_root) {
        std::stack<node*> nodes;
        nodes.push(m_root);

        while (!nodes.empty()) {
            auto current = nodes.top(); nodes.pop();
            if (current->left) {
                nodes.push(current->left);
            }
            if (current->right) {
                nodes.push(current->right);
            }
            delete current;
        }
    }
    m_root = nullptr;
    for(auto ele : rhs.preorder_elements()){
        add(ele);
    }
    return (*this);
};

BST::BST(BST&& rhs) {
    m_root = rhs.m_root;
    rhs.m_root = nullptr;
};

BST& BST::operator=(BST&& rhs) {
    if(this == &rhs) return *this;
    if (m_root) {
        std::stack<node*> nodes;
        nodes.push(m_root);

        while (!nodes.empty()) {
            auto current = nodes.top(); nodes.pop();
            if (current->left) {
                nodes.push(current->left);
            }
            if (current->right) {
                nodes.push(current->right);
            }
            delete current;
        }
    }
    m_root = rhs.m_root;
    rhs.m_root = nullptr;
    return (*this);
};

BST::~BST() {
    if (m_root) {
        std::stack<node*> nodes;
        nodes.push(m_root);

        while (!nodes.empty()) {
            auto current = nodes.top(); nodes.pop();
            if (current->left) {
                nodes.push(current->left);
            }
            if (current->right) {
                nodes.push(current->right);
            }
            delete current;
        }
    }
}
