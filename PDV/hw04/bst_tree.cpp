#include <functional>
#include <atomic>
#include "bst_tree.h"

void bst_tree::insert(long long data) {
    node * nullnode = nullptr;
    auto * newnode = new node(data);
    if(root == nullnode){
        if(root.compare_exchange_strong(nullnode, newnode)) {
            return;
        }
    }
    node * tmp = root;
    while(true){
        if ( tmp->data < data ){
            if( tmp->right != nullptr ){
                tmp = tmp->right;
            } else if(tmp->right.compare_exchange_strong(nullnode, newnode)){
                break;
            }
        }
        else {
            if( tmp->left != nullptr ){
                tmp = tmp->left;
            } else if (tmp->left.compare_exchange_strong(nullnode, newnode)) {
                break;
            }
        }
    }
}

bst_tree::~bst_tree() {
    std::function<void(node*)> cleanup = [&](node * n) {
        if(n != nullptr) {
            cleanup(n->left);
            delete n;
            cleanup(n->right);
        }
    };
    cleanup(root);
}
