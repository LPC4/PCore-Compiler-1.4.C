#include "../include/Parser.h"
#include "../include/ast/AbstractSyntaxTree.h"

Parser::Parser() = default;

void Parser::parse(std::vector<Token> tokens) {
    m_tokens = std::move(tokens);
    parseProgram();
}


