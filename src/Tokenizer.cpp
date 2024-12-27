#include "../include/Tokenizer.h"

#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>

Tokenizer::Tokenizer(const std::string &filepath) : m_filepath(filepath) {}

auto Tokenizer::tokenize() -> std::vector<Token> {
    m_source = readSource();
    m_line = 1;
    m_column = 1;
    m_max_index = m_source.size();
    m_current_index = 0;

    while (m_current_index < m_max_index) {
        char currentChar = m_source[m_current_index];

        if (WHITESPACE.contains(currentChar)) {
            handleWhiteSpace();
        } else if (isalpha(currentChar) || currentChar == '_') {
            handleIdentifierOrKeyword();
        } else if (isdigit(currentChar)) {
            handleNumber();
        } else if (currentChar == '"' || currentChar == '\'') {
            handleStringOrChar();
        } else if (SYMBOLS.contains(currentChar)) {
            handleSymbol();
        } else {
            throwError("Tokenizer: invalid character");
        }
    }

    return m_tokens;
}

void Tokenizer::handleWhiteSpace() {
    while (m_current_index < m_max_index && WHITESPACE.contains(m_source[m_current_index])) {
        if (m_source[m_current_index] == '\n') {
            m_line++;
            m_column = 1;
        } else {
            m_column++;
        }
        m_current_index++;
    }
}

void Tokenizer::handleKeyword() {
    std::string keyword;
    while (m_current_index < m_max_index && isalpha(m_source[m_current_index])) {
        keyword += m_source[m_current_index];
        advance();
    }
    addToken(TokenType::Keyword, keyword);
}

void Tokenizer::handleSymbol() {
    if (m_current_index + 1 < m_max_index) {
        const std::string symbol = m_source.substr(m_current_index, 2);

        if (symbol == "//") {
            while (m_current_index < m_max_index && m_source[m_current_index] != '\n') {
                advance();
            }
            return;
        }

        if (LONG_SYMBOLS.contains(symbol)) {
            addToken(TokenType::Symbol, symbol);
            advance();
            advance();
            return;
        }
    }

    std::string symbol(1, m_source[m_current_index]);
    addToken(TokenType::Symbol, symbol);
    advance();
}

void Tokenizer::handleStringOrChar() {
    std::string value;
    const char  quote = m_source[m_current_index];
    advance();

    while (m_current_index < m_max_index && m_source[m_current_index] != quote) {
        value += m_source[m_current_index];
        advance();
    }

    if (m_current_index >= m_max_index) {
        throwError("Tokenizer: unterminated string or char literal");
    }

    advance(); // Consume the closing quote

    if (quote == '"') {
        addToken(TokenType::String, value);
    } else {
        if (value.size() != 1) {
            throwError("Tokenizer: invalid char");
        }
        addToken(TokenType::Char, value);
    }
}

void Tokenizer::handleNumber() {
    std::string number;
    bool        isFloat = false;

    while (m_current_index < m_max_index && (isdigit(m_source[m_current_index]) || m_source[m_current_index] == '.')) {
        if (m_source[m_current_index] == '.') {
            if (isFloat) {
                throwError("Tokenizer: multiple periods in float");
            }
            isFloat = true;

            // Check if there's a digit after the '.'
            if (m_current_index + 1 >= m_max_index || !isdigit(m_source[m_current_index + 1])) {
                throwError("Tokenizer: invalid float format");
            }
        }

        number += m_source[m_current_index];
        advance();
    }

    addToken(isFloat ? TokenType::Float : TokenType::Integer, number);
}

void Tokenizer::handleIdentifierOrKeyword() {
    std::string identifier;

    while (m_current_index < m_max_index && (isalnum(m_source[m_current_index]) || m_source[m_current_index] == '_')) {
        identifier += m_source[m_current_index];
        advance();
    }

    if (KEYWORDS.contains(identifier)) {
        addToken(TokenType::Keyword, identifier);
    } else {
        addToken(TokenType::Identifier, identifier);
    }
}

void Tokenizer::advance() {
    if (m_current_index >= m_max_index) {
        throwError("Tokenizer: cannot advance past end of source");
    }
    m_current_index++;
    m_column++;
}

void Tokenizer::addToken(TokenType type, const std::string &value) {
    m_tokens.emplace_back(value, type, Position(m_line, m_column));
}

void Tokenizer::throwError(const std::string &message) const {
    throw std::runtime_error(message + " at line " + std::to_string(m_line) + " column " + std::to_string(m_column) +
                             " (character: " +
                             (m_current_index < m_max_index ? std::string(1, m_source[m_current_index]) : "EOF") + ")");
}

auto Tokenizer::readSource() const -> std::string {
    std::ifstream file(m_filepath);
    std::string   source;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            source += line + '\n';
        }
    } else {
        throw std::runtime_error("Failed to open file");
    }

    return source;
}

Token::Token(std::string value, const TokenType type, const Position position) :
    m_type(type), m_value(std::move(value)), m_position(position) {}

Position::Position(const unsigned int line, const unsigned int column) : m_line(line), m_column(column) {}
