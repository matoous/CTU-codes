#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

class Sequential {
public:
    class Node {
    public:
        long long value;
        Node * next = nullptr;

        Node(long long value) : value(value) {}
    };

    Node * head = new Node(-999999999999L);

    void add(long long value) {
        Node * node = new Node(value);

        if(head == nullptr) head = node;
        else {
            Node * current = head;
            while(current->next != nullptr && current->next->value < value) {
                current = current->next;
            }
            node->next = current->next;
            current->next = node;
        }
    }
};

#endif
