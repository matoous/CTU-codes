#ifndef LOCKFREE_H
#define LOCKFREE_H

#include <vector>
#include <iostream>
#include <atomic>

class Lockfree {
public:
    class Node {
    public:
        std::atomic<unsigned long long> value;
        std::atomic<Node*> next { nullptr };

        Node(unsigned long long value) : value(value) {}
    };

    static constexpr unsigned long long mask = 1L << 63;
    Node * head = new Node(42L | mask);

    void add(unsigned long long value) {
        Node * node = new Node(value);

        Node * current = head;
        Node * next = current->next;
        while(true) {
            if(next == nullptr || next->value > node->value){
                break;
            }
            current = next;
            next = next->next;
        }

        node->next = next;
        current->next = node;

    }

    bool remove(unsigned long long value) {
        return false;
    }
};

#endif