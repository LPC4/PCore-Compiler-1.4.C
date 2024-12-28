#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include "AbstractSyntaxTree.h"
#include "Tokenizer.h"

class Parser {
public:
    Parser();

    // Main parse function
    std::unique_ptr<Program> parse(std::vector<Token> tokens);

private:
    std::vector<Token> m_tokens;
    std::size_t        m_current_index = 0;

    // --------------------- Parsing functions --------------------- //

    std::unique_ptr<Program>             parseProgram();
    std::unique_ptr<Block>               parseBlock();
    std::unique_ptr<AbstractNode>        parseDeclaration();
    std::unique_ptr<AbstractNode>        parseStatement();
    std::unique_ptr<IfStatement>         parseIfStatement();
    std::unique_ptr<WhileLoop>           parseWhileLoop();
    std::unique_ptr<ReturnStatement>     parseReturnStatement();
    std::unique_ptr<AbstractNode>        parseExpression();
    std::unique_ptr<AbstractNode>        parsePrimaryExpression();
    std::unique_ptr<AbstractNode>        parseUnaryExpression();
    std::unique_ptr<VariableDeclaration> parseVariableDeclaration();
    std::unique_ptr<FunctionCall>        parseFunctionCall();
    std::unique_ptr<FunctionDeclaration> parseFunctionDeclaration();
    std::unique_ptr<AbstractNode>        parseBinaryOperation(int precedence = 0);
    std::unique_ptr<Assignment>          parseAssignment();

    // ------------------ Parsing Helper Functions ------------------ //

    void advance();

    // Advances if the current token matches, otherwise throws an error
    void consume(TokenType type);
    void consume(const std::string &value);
    void consume(TokenType type, const std::string &value);

    // Returns true if the current token matches
    [[nodiscard]] bool match(TokenType type) const;
    [[nodiscard]] bool match(const std::string &value) const;
    [[nodiscard]] bool match(TokenType type, const std::string &value) const;

    // Returns tokens without advancing
    [[nodiscard]] Token              peek() const;
    [[nodiscard]] Token              peekNext() const;
    [[nodiscard]] Token              peekPrevious() const;
    [[nodiscard]] std::vector<Token> peekUntilEOL();

    [[nodiscard]] bool isAtEnd() const;

    [[noreturn]] void throwError(const std::string &message) const;

    // ------------------ Static Helper Functions ------------------ //

    [[nodiscard]] static bool isBinaryOperator(const Token &peek);
    [[nodiscard]] static bool isBinaryOperation(const std::vector<Token> &line);
    [[nodiscard]] static int  getOperatorPrecedence(const std::string &string);
};

inline auto Parser::peek() const -> Token { return m_tokens[m_current_index]; }

inline auto Parser::peekNext() const -> Token { return m_tokens[m_current_index + 1]; }

inline auto Parser::peekPrevious() const -> Token { return m_tokens[m_current_index - 1]; }

inline auto Parser::peekUntilEOL() -> std::vector<Token> {
    std::vector<Token> tokens;
    const std::size_t  index = m_current_index;

    while (!isAtEnd() && peek().getValue() != ";") {
        tokens.push_back(peek());
        ++m_current_index;
    }
    m_current_index = index;
    return tokens;
}

inline void Parser::advance() {
    if (isAtEnd()) {
        throwError("cannot advance past end of token stream");
    }
    ++m_current_index;
}

inline auto Parser::isAtEnd() const -> bool { return m_current_index >= m_tokens.size(); }

inline auto Parser::match(const TokenType type) const -> bool {
    return !isAtEnd() && m_tokens[m_current_index].getType() == type;
}

inline auto Parser::match(const std::string &value) const -> bool {
    return !isAtEnd() && m_tokens[m_current_index].getValue() == value;
}

inline auto Parser::match(const TokenType type, const std::string &value) const -> bool {
    return !isAtEnd() && m_tokens[m_current_index].getType() == type && m_tokens[m_current_index].getValue() == value;
}

inline void Parser::consume(const TokenType type) {
    match(type) ? advance() : throwError("expected token of type " + tokenTypeToString(type));
}

inline void Parser::consume(const std::string &value) {
    match(value) ? advance() : throwError("expected token with value " + value);
}

inline void Parser::consume(const TokenType type, const std::string &value) {
    match(type, value) ? advance()
                       : throwError("expected token of type " + tokenTypeToString(type) + " with value " + value);
}

inline void Parser::throwError(const std::string &message) const {
    throw std::runtime_error("Parse error: " + message + " at line " + std::to_string(peek().getPosition().getLine()) +
                             " column " + std::to_string(peek().getPosition().getColumn()) + " (token: \"" +
                             peek().getValue() + "\")");
}

inline auto Parser::isBinaryOperation(const std::vector<Token> &line) -> bool {
    return std::ranges::any_of(line, [](const Token &token) { return BINARY_OPERATORS.contains(token.getValue()); });
}

inline auto Parser::isBinaryOperator(const Token &peek) -> bool { return BINARY_OPERATORS.contains(peek.getValue()); }

inline auto Parser::getOperatorPrecedence(const std::string &op) -> int {
    static const std::unordered_map<std::string, int> PRECEDENCE_TABLE = {
            {"=", 1},  // Assignment
            {"||", 2}, // Logical OR
            {"&&", 3}, // Logical AND
            {"|", 4},  // Bitwise OR
            {"^", 5},  // Bitwise XOR
            {"&", 6},  // Bitwise AND
            {"==", 7}, // Equality
            {"!=", 7}, // Inequality
            {"<", 8},  // Relational
            {">", 8},  // Relational
            {"<=", 8}, // Relational
            {">=", 8}, // Relational
            {"+", 9},  // Addition
            {"-", 9},  // Subtraction
            {"*", 10}, // Multiplication
            {"/", 10}, // Division
            {"%", 10}, // Modulo
            {"-", 11}, // Unary negation (handled separately in unary parsing)
            {"!", 11}, // Logical NOT (handled separately in unary parsing)
    };

    const auto it = PRECEDENCE_TABLE.find(op);
    if (it != PRECEDENCE_TABLE.end()) {
        return it->second;
    }

    throw std::runtime_error("invalid operator " + op);
}
