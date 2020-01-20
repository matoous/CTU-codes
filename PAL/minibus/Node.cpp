#include "Node.h"

Node::Node(int id) {
    this->id = id;
}

bool Node::operator==(const Node &n1) {
    return this->id == n1.id;
}
