#ifndef LOCKBASED_H
#define LOCKBASED_H

#include <vector>
#include <iostream>
#include <atomic>

class spin_mutex {
private:
    std::atomic_flag flag;

public:
    spin_mutex() : flag(ATOMIC_FLAG_INIT) {}

    void lock() {
        while(flag.test_and_set(std::memory_order_acq_rel));
    }

    void unlock() {
        flag.clear();
    }

};

class Concurrent {
public:
    class Node {
    public:
        long long value;
        Node * next = nullptr;
        spin_mutex m;

        Node(long long value) : value(value) {}
    };

    Node * head = new Node(-999999999999L);
    spin_mutex head_mutex;

    void add(long long value) {
        auto * node = new Node(value);

        Node * curr = head;
        while(true){
            Node * next = curr->next;
            if(next == nullptr || next->value > value){
                node->next = next;
                if(curr){

                }
            }
        }
    }

    void remove(long long value) {
    }
};

#endif