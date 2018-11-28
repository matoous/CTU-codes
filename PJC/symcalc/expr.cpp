#include "expr.hpp"
#include "expr_impl.hpp"
#include <stdexcept>

const expr expr::ZERO = expr::number(0.0);
const expr expr::ONE = expr::number(1.0);

// TODO: overloaded operators +, -, *, /, functions pow, log, sin, cos,
//       expr::number, expr::variable, operator==, operator<<,
//       create_expression_tree

expr create_expression_tree(const std::string& expression) {
    // TODO
    throw std::logic_error("not implemented");
}

bool operator==(const expr &a, const expr &b) {
    // TODO
    throw std::logic_error("not implemented");
}

std::ostream& operator<<(std::ostream &os, const expr &e) {
    // TODO
    throw std::logic_error("not implemented");
}
