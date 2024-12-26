#pragma once

#include <filesystem>
#include <string>
#include <vector>

// Enumeration for different token types
enum class TokenType : std::uint8_t {
    Identifier, // Function names, variable names, etc.
    Integer,    // Example: 1
    Float,      // Example: 1.0
    String,     // Double-quoted strings: "example"
    Char,       // Single characters: 'a'
    Keyword,    // Reserved keywords: if, else, while, etc.
    Symbol      // Operators and symbols: +, -, *, /, =, etc.
};

// Represents a position in the source code
class Position {
public:
    Position(unsigned int line, unsigned int column);

    [[nodiscard]] auto getLine() const -> unsigned int;
    [[nodiscard]] auto getColumn() const -> unsigned int;

private:
    unsigned int m_line;
    unsigned int m_column;
};

// Represents a single token
class Token {
public:
    Token(std::string value, TokenType type, Position position);

    [[nodiscard]] auto getType() const -> TokenType;
    [[nodiscard]] auto getValue() const -> const std::string &;
    [[nodiscard]] auto getPosition() const -> const Position &;

private:
    TokenType m_type;
    std::string m_value;
    Position m_position;
};

// Tokenizer class for processing source files into tokens
class Tokenizer {
public:
    explicit Tokenizer(const std::string &filepath);

    auto tokenize() -> std::vector<Token>;

    [[nodiscard]] auto getTokens() const -> const std::vector<Token> &;

private:
    std::filesystem::path m_filepath;
    std::string m_source;
    std::vector<Token> m_tokens;

    long long unsigned int m_max_index = 0;
    long long unsigned int m_current_index = 0;

    unsigned int m_line = 1;
    unsigned int m_column = 1;

    void handleWhiteSpace();
    void handleKeyword();
    void handleSymbol();
    void handleStringOrChar();
    void handleNumber();
    void handleIdentifierOrKeyword();

    void advance();
    void addToken(TokenType type, const std::string &value);
    void throwError(const std::string &message) const;

    [[nodiscard]] auto readSource() const -> std::string;
};

inline auto Tokenizer::getTokens() const -> const std::vector<Token> & { return m_tokens; }

inline auto Token::getType() const -> TokenType { return m_type; }

inline auto Token::getValue() const -> const std::string & { return m_value; }

inline auto Token::getPosition() const -> const Position & { return m_position; }

inline auto Position::getLine() const -> unsigned int { return m_line; }

inline auto Position::getColumn() const -> unsigned int { return m_column; }
