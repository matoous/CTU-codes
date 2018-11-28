#include "tokenizer.hpp"
#include "expr.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cctype>

//------------------------------------------------------------------------------
//---  Token
//------------------------------------------------------------------------------

bool Token::is_binary_op() const {
    return id != TokenId::End && std::strchr(BINOPS, static_cast<char>(id)) != nullptr;
}
//------------------------------------------------------------------------------

int Token::op_precedence() const {
    assert(is_binary_op());
    switch (id)
    {
        case TokenId::Power:
            return 3;
        case TokenId::Multiply:
        case TokenId::Divide:
            return 2;
        default:
            return 1;
    }
}
//------------------------------------------------------------------------------

Associativity Token::associativity() const {
    assert(is_binary_op());
    if (id == TokenId::Power)
        return Associativity::Right;
    return Associativity::Left;
}
//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream &os, const Token &tok)
{
    os << "Token('" << (char)tok.id << "'";
    if(tok.id == TokenId::Number)
        os << ", " << tok.number;
    else if(tok.id == TokenId::Identifier)
        os << ", " << tok.identifier;
    os << ")";
    return os;
}
//------------------------------------------------------------------------------
//---  Tokenizer
//------------------------------------------------------------------------------

Tokenizer::Tokenizer(std::istream &is)
    : m_is(is)
{
}
//------------------------------------------------------------------------------

Token Tokenizer::next()
{
    m_is >> std::skipws;
    std::istream::sentry sentry(m_is);
    if (!sentry)
        return Token(TokenId::End);

    auto c = m_is.peek();
    Token res;
    bool consume = true;

    if (c == std::istream::traits_type::eof())
        return Token(TokenId::End);
    else if (std::isdigit(c)) {
        double number;
        m_is >> number;
        res = Token(TokenId::Number, number);
        consume = false;
    } else if (std::isalpha(c)) {
        std::string ident;
        m_is.ignore();
        do {
            ident.push_back(c);
            c = m_is.get();
        } while (std::isalpha(c));
        if (c != std::istream::traits_type::eof())
            m_is.putback(c);

        // clearly retarded API (pun intended): clear does not clear the bits,
        // but sets them
        m_is.clear(m_is.rdstate() & ~std::ios::failbit);

        res = Token(TokenId::Identifier, std::move(ident));
        consume = false;
    } else {
        if (std::strchr(SYMBOL_TOKENS, c) == nullptr) {
            std::stringstream ss;
            ss << "unknown token " << static_cast<char>(c);
            throw tokenize_error(ss.str());
        }
        res = Token(static_cast<TokenId>(c));
    }
    if (m_is.fail())
        throw tokenize_error("failed conversion");
    if (consume)
        m_is.get();
    return res;
}
