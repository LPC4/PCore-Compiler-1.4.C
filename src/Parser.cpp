#include "../include/Parser.h"

Parser::Parser() = default;

void Parser::parse(std::vector<Token> tokens) {
    this->tokens = tokens;
    m_max_index = tokens.size();
    m_current_index = 0;

    while (!isAtEnd()) {
        Token token = peek();

        if (consume(TokenType::Keyword, "program")) {
            consume(TokenType::Identifier);
            consume(TokenType::Symbol, "{");

            while (!consume(TokenType::Symbol, "}")) {
                parseStatement();
            }
        } else {
            throwError("Parser: expected 'program' keyword");
        }
    }
}