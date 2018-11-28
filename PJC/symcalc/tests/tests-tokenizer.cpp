#include "catch.hpp"
#include "../tokenizer.hpp"
#include "../expr.hpp"

#include <sstream>
#include <vector>

namespace {
    std::vector<Token> tokenize(std::string s)
    {
        std::stringstream ss(s);
        Tokenizer t(ss);
        std::vector<Token> tokens;

        Token token;
        do {
            token = t.next();
            tokens.push_back(token);
        } while (token.id != TokenId::End);
        return tokens;
    }
}

using token_vector = std::vector<Token>;

TEST_CASE("Tokenizer") {
    // empty
    CHECK(tokenize("") == token_vector{
        Token(TokenId::End),
    });

    CHECK(tokenize("42.38e-6") == token_vector{
        Token(TokenId::Number, 42.38e-6),
        Token(TokenId::End),
    });

    CHECK(tokenize("asdQQQ") == token_vector{
        Token(TokenId::Identifier, "asdQQQ"),
        Token(TokenId::End),
    });

    CHECK(tokenize("()+-*/^") == token_vector{
        Token(TokenId::LParen),
        Token(TokenId::RParen),
        Token(TokenId::Plus),
        Token(TokenId::Minus),
        Token(TokenId::Multiply),
        Token(TokenId::Divide),
        Token(TokenId::Power),
        Token(TokenId::End),
    });

    CHECK(tokenize("myfunc(myvar+123)") == token_vector{
        Token(TokenId::Identifier, "myfunc"),
        Token(TokenId::LParen),
        Token(TokenId::Identifier, "myvar"),
        Token(TokenId::Plus),
        Token(TokenId::Number, 123),
        Token(TokenId::RParen),
        Token(TokenId::End),
    });

    CHECK_THROWS_AS(tokenize("#"), tokenize_error);

    // whitespace should be ignored
    CHECK(tokenize("a   + \t  f \n (") == token_vector{
        Token(TokenId::Identifier, "a"),
        Token(TokenId::Plus),
        Token(TokenId::Identifier, "f"),
        Token(TokenId::LParen),
        Token(TokenId::End),
    });
}
