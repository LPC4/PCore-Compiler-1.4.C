#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include "Tokenizer.h"
#include "ast/AbstractSyntaxTree.h"

class Parser {
public:
    Parser();

    // Main parse function
    void parse(std::vector<Token> tokens);

private:
    std::vector<Token> m_tokens;
    std::size_t        m_current_index = 0;

    // --------------------- Parsing functions --------------------- //

    std::unique_ptr<Program>                   parseProgram();
    std::unique_ptr<AbstractNode>              parseDeclaration();
    std::unique_ptr<AbstractNode>              parseStatement();
    std::vector<std::unique_ptr<AbstractNode>> parseBlock();
    std::unique_ptr<IfStatement>               parseIfStatement();
    std::unique_ptr<WhileLoop>                 parseWhileLoop();
    std::unique_ptr<ReturnStatement>           parseReturnStatement();
    std::unique_ptr<ExpressionStatement>       parseExpressionStatement();
    std::unique_ptr<VariableDeclaration>       parseVariableDeclaration();
    std::unique_ptr<FunctionCall>              parseFunctionCall();
    std::unique_ptr<FunctionDeclaration>       parseFunctionDeclaration();
    std::unique_ptr<BinaryOperation>           parseBinaryOperation();
    std::unique_ptr<Assignment>                parseAssignment();

    // ------------------ Parsing Helper Functions ------------------ //

    void advance();

    // Advances if the current token matches, otherwise throws an error
    void consume(TokenType type);
    void consume(const std::string &value);
    void consume(TokenType type, const std::string &value);

    void throwError(const std::string &message) const;

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

    [[nodiscard]] static bool isBinaryOperation(const std::vector<Token> &line);
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
        throwError("Parser: cannot advance past end of token stream");
    } else {
        ++m_current_index;
    }
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
    match(type) ? advance() : throwError("Parser: expected token of type " + tokenTypeToString(type));
}

inline void Parser::consume(const std::string &value) {
    match(value) ? advance() : throwError("Parser: expected token with value " + value);
}

inline void Parser::consume(TokenType type, const std::string &value) {
    match(type, value)
            ? advance()
            : throwError("Parser: expected token of type " + tokenTypeToString(type) + " with value " + value);
}

inline void Parser::throwError(const std::string &message) const {
    throw std::runtime_error("Parse error: " + message + " at line " + std::to_string(peek().getPosition().getLine()) +
                             " column " + std::to_string(peek().getPosition().getColumn()) +
                             " (token: " + peek().getValue() + ")");
}

inline auto Parser::isBinaryOperation(const std::vector<Token>& line) -> bool {
    return std::ranges::any_of(line, [](const Token& token) {
        return BINARY_OPERATORS.contains(token.getValue());
    });
}
