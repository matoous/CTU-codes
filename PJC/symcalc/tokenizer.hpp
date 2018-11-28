#pragma once
#include <iosfwd>
#include <string>
#include <cassert>

enum class TokenId : char
{
    Number = '0',
    Identifier = 'a',
    LParen = '(',
    RParen = ')',
    Plus = '+',
    Minus = '-',
    Multiply = '*',
    Divide = '/',
    Power = '^',
    End = 0
};
#define BINOPS "+-*/^"
#define SYMBOL_TOKENS BINOPS "()"

enum class Associativity {
    Left, Right
};

struct Token
{
    TokenId id;
    //union {
    double number = 0.0;
    std::string identifier;
    //};

    explicit Token(TokenId id) : id(id) {}
    Token(TokenId id, double number) : id(id), number(number) {
        assert(id == TokenId::Number);
    }
    Token(TokenId id, std::string ident) : id(id), identifier(std::move(ident)) {
        assert(id == TokenId::Identifier);
    }
    Token() : id(TokenId::End) {}

    bool is_binary_op() const;
    int op_precedence() const;
    Associativity associativity() const;
};

static inline bool operator==(const Token &a, const Token &b) {
    return a.id == b.id && (a.id != TokenId::Number || a.number == b.number);
}

static inline bool operator!=(const Token &a, const Token &b) {
    return !(a == b);
}

std::ostream& operator<<(std::ostream &os, const Token &tok);

/**
 * Z daného streamu parsuje jednotlivé tokeny. Při chybě vyhodí tokenize_error.
 */
class Tokenizer
{
public:
    explicit Tokenizer(std::istream &is);
    Token next();
private:
    std::istream &m_is;
};
