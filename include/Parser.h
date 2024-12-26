#pragma once

#include <string>
#include <vector>
#include "Tokenizer.h"

class Parser {
public:
    Parser();

    // Main parse function
    void parse(std::vector<Token> tokens);

private:
    std::vector<Token> m_tokens;     // Vector to hold tokens
    std::size_t m_current_index = 0; // Current index in the token vector

    // Parsing functions for various constructs
    void parseProgram();
    void parseStatement();
    void parseExpression();
    void parseAssignment();
    void parseFunctionCall();
    void parseControlFlow();
    void parseLoop();
    void parseReturn();

    // Utility functions
    void advance();

    // Advances if the current token matches, otherwise throws an error
    void consume(TokenType type);
    void consume(const std::string &value);
    void consume(TokenType type, const std::string &value);

    // Throw an error with a specific message
    void throwError(const std::string &message);

    // Returns true if the current token matches
    [[nodiscard]] bool match(TokenType type) const;
    [[nodiscard]] bool match(const std::string &value) const;
    [[nodiscard]] bool match(TokenType type, const std::string &value) const;

    // Returns the current token
    [[nodiscard]] Token peek() const;
    [[nodiscard]] Token peekNext() const;
    [[nodiscard]] Token peekPrevious() const;

    // Check if the end of the token stream is reached
    [[nodiscard]] bool isAtEnd() const;
};

inline auto tokenTypeToString(const TokenType type) -> std::string {
    switch (type) {
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::Integer:
            return "Integer";
        case TokenType::Float:
            return "Float";
        case TokenType::String:
            return "String";
        case TokenType::Char:
            return "Char";
        case TokenType::Keyword:
            return "Keyword";
        case TokenType::Symbol:
            return "Symbol";
        default:
            return "Unknown";
    }
}

inline auto Parser::peek() const -> Token { return m_tokens[m_current_index]; }

inline auto Parser::peekNext() const -> Token { return m_tokens[m_current_index + 1]; }

inline auto Parser::peekPrevious() const -> Token { return m_tokens[m_current_index - 1]; }

inline auto Parser::isAtEnd() const -> bool { return m_current_index >= m_tokens.size(); }

inline void Parser::advance() {
    if (!isAtEnd()) {
        ++m_current_index;
    }
}

inline auto Parser::match(const TokenType type) const -> bool { return !isAtEnd() && m_tokens[m_current_index].getType() == type; }

inline auto Parser::match(const std::string &value) const -> bool {
    return !isAtEnd() && m_tokens[m_current_index].getValue() == value;
}

inline auto Parser::match(const TokenType type, const std::string &value) const -> bool {
    return !isAtEnd() && m_tokens[m_current_index].getType() == type && m_tokens[m_current_index].getValue() == value;
}

inline void Parser::consume(const TokenType type) {
    if (match(type)) {
        advance();
    } else {
        throwError("Parser: expected token of type " + tokenTypeToString(type));
    }
}

inline void Parser::consume(const std::string &value) {
    if (match(value)) {
        advance();
    } else {
        throwError("Parser: expected token with value " + value);
    }
}

inline void Parser::consume(TokenType type, const std::string &value) {
    if (match(type, value)) {
        advance();
    } else {
        throwError("Parser: expected token of type " + tokenTypeToString(type) + " with value " + value);
    }
}

inline void Parser::throwError(const std::string &message) { throw std::runtime_error("Parse error: " + message); }
