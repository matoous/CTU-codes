#ifndef PDV_HW03_TESTS_H
#define PDV_HW03_TESTS_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>
#include <cstdlib>

#include "bst_tree.h"

template <unsigned int N>
class base_test {
public:
    std::vector<long long> data;
    bst_tree tree;

    base_test() : data(N) {
        std::iota(data.begin(), data.end(), 0);
    }

    void run_test() {
        #pragma omp parallel for schedule(dynamic)
        for(unsigned int i = 0 ; i < N ; i++) {
            tree.insert(data[i]);
        }
    }

    bool verify() {
        std::vector<long long> content;
        std::function<void(bst_tree::node*)> inorder = [&](bst_tree::node * node) {
            if(node != nullptr) {
                inorder(node->left);
                content.push_back(node->data);
                inorder(node->right);
            }
        };
        inorder(tree.root);
        if(content.size() != N) return false;
        for(unsigned int i = 0 ; i < N ; i++) {
            if(content[i] != i) return false;
        }
        return true;
    }
};

template <unsigned int N>
class shuffled_data : public base_test<N> {
public:
    shuffled_data() {
        srand(0L);
        std::random_shuffle(this->data.begin(), this->data.end());
    }
};

template <unsigned int N>
class sorted_data : public base_test<N> {
};

#endif //PDV_HW03_TESTS_H
