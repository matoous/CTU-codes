#include "expr.hpp"
#include "tokenizer.hpp"
#include "expr_impl.hpp"
#include <stdexcept>
#include <deque>
#include <iostream>
#include <sstream>
#include <stack>

const expr expr::ZERO = expr::number(0.0);
const expr expr::ONE = expr::number(1.0);

class expr_queue {
private:
    std::deque<expr> output;

public:
    expr_queue() {
        output = std::deque<expr>();
    }

    void push(const expr& e){
        output.push_back(e);
    }

    const expr& back() {
        return output.back();
    }

    expr result() {
        return output.front();
    }

    void apply_token(Token& t){
        expr x,y;
        switch (t.id) {
            case TokenId::Plus:
                x = output.back(); output.pop_back();
                y = output.back(); output.pop_back();
                output.push_back(y + x);
                break;
            case TokenId::Minus:
                x = output.back(); output.pop_back();
                y = output.back(); output.pop_back();
                output.push_back(y - x);
                break;
            case TokenId::Multiply:
                x = output.back(); output.pop_back();
                y = output.back(); output.pop_back();
                output.push_back(y * x);
                break;
            case TokenId::Divide:
                x = output.back(); output.pop_back();
                y = output.back(); output.pop_back();
                output.push_back(y / x);
                break;
            case TokenId::Power:
                x = output.back(); output.pop_back();
                y = output.back(); output.pop_back();
                output.push_back(pow(y,x));
                break;
            case TokenId::Identifier:
                if(t.is_function()){
                    x = output.back(); output.pop_back();
                    if(t.identifier == "sin") output.push_back(sin(x));
                    if(t.identifier == "cos") output.push_back(cos(x));
                    if(t.identifier == "log") output.push_back(log(x));
                }
                break;
            default:
                throw tokenize_error("attempt to apply unknown thing");
        }
    }

};

// TODO: overloaded operators +, -, *, /, functions pow, log, sin, cos,
//       expr::number, expr::variable, operator==, operator<<,

expr create_expression_tree(const std::string& expression) {
    auto xxx = std::stringstream(expression);
    auto tknzr = Tokenizer(xxx);

    auto output = expr_queue();
    auto stack = std::stack<Token>();
    bool needs_parenthesis = false;

    while(true){
        try {
            auto token = tknzr.next();
            if(needs_parenthesis && token.id != TokenId::LParen)
                throw tokenize_error("missing parenthesis for function");
            switch (token.id){
                case TokenId::End:
                    while(!stack.empty()){
                        if(stack.top().id == TokenId::LParen || stack.top().id == TokenId::RParen){
                            throw tokenize_error("failed conversion");
                        } else {
                            output.apply_token(stack.top());
                            stack.pop();
                        }
                    }
                    break;
                case TokenId::Number:
                    output.push(expr::number(token.number));
                    break;
                case TokenId::Identifier:
                    if(token.identifier == "sin" || token.identifier == "cos" || token.identifier == "log"){
                        needs_parenthesis = true;
                        stack.push(token);
                    } else {
                        output.push(expr::variable(token.identifier));
                    }
                    break;
                case TokenId::LParen:
                    if(needs_parenthesis) needs_parenthesis = false;
                    stack.push(token);
                    break;
                case TokenId::RParen:
                    while(stack.top().id != TokenId::LParen){
                        output.apply_token(stack.top());
                        stack.pop();
                    }
                    // pop the left bracket
                    stack.pop();
                    break;
                case TokenId::Plus: case TokenId::Minus: case TokenId::Multiply: case TokenId::Divide: case TokenId::Power:
                    while(
                            !stack.empty()
                            && (stack.top().is_function()
                                || (stack.top().is_binary_op() && stack.top().op_precedence() > token.op_precedence())
                                || (stack.top().is_binary_op() && stack.top().op_precedence() == token.op_precedence() && stack.top().associativity() == Associativity::Left))
                            && stack.top().id != TokenId::LParen)
                    {
                        output.apply_token(stack.top());
                        stack.pop();
                    }
                    stack.push(token);
                    break;
            }
            if(token.id == TokenId::End) break;
        }
        catch (const tokenize_error &e) {
            throw e;
        }
    }

    return output.result();
}

bool operator==(const expr &a, const expr &b) {
    return a->equals(b->shared_from_this().operator*());
}

expr expr::number(double n) {
    return std::make_shared<exprs::number>(exprs::number(n));
}

expr expr::variable(std::string name) {
    return std::make_shared<exprs::variable>(exprs::variable(name));
}

std::ostream& operator<<(std::ostream &os, const expr &e) {
    e->write(os, expr::WriteFormat::Infix);
    return os;
}

std::ostream &operator<<(std::ostream &os, const fmt_expr &e) {
    e.e->write(os, e.fmt);
    return os;
}

expr operator+(expr a, expr b) {
    return std::make_shared<exprs::expr_plus>(exprs::expr_plus(a, b));
}

expr operator-(expr a, expr b) {
    return std::make_shared<exprs::expr_minus>(exprs::expr_minus(a, b));
}

expr operator*(expr a, expr b) {
    return std::make_shared<exprs::expr_multiply>(exprs::expr_multiply(a, b));
}

expr operator/(expr a, expr b) {
    return std::make_shared<exprs::expr_divide>(exprs::expr_divide(a, b));
}

expr pow(expr m, expr e) {
    return std::make_shared<exprs::expr_pow>(exprs::expr_pow(m, e));
}

expr sin(expr e) {
    return std::make_shared<exprs::expr_sin>(exprs::expr_sin(e));
}

expr cos(expr e) {
    return std::make_shared<exprs::expr_cos>(exprs::expr_cos(e));
}

expr log(expr e) {
    return std::make_shared<exprs::expr_log>(exprs::expr_log(e));
}
