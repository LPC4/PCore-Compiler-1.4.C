#include "../include/Parser.h"
#include "../include/ast/AbstractSyntaxTree.h"

Parser::Parser() = default;

void Parser::parse(std::vector<Token> tokens) {
    m_tokens = std::move(tokens);
    parseProgram();
}

auto Parser::parseProgram() -> Program {
    // program name
    consume(TokenType::Keyword, "program");

    const std::string programName = peek().getValue();
    printf("Program name: %s\n", programName.c_str());

    consume(TokenType::Identifier);
    consume(";");

    // program body
    while (!isAtEnd()) {
        parseStatement();
    }

    return Program(programName, nullptr);
}

auto Parser::parseStatement() -> std::unique_ptr<ASTNode> {
    // Statements:
    // - Function declaration can start with either
    //  - (             # params and return type explicitly defined
    //  - func          # params and return type inferred (void)
    //  - identifier {  # params and return type inferred (void)
    // - Variable declaration starts with
    //  - identifier    # variable declaration with explicit typing

    if (match(TokenType::Symbol, "(") || match(TokenType::Keyword, "func") ||
        match(TokenType::Identifier) && peekNext().getValue() == "{") {

        return parseFunctionDeclaration();
    }
    if (match(TokenType::Identifier)) {
        return parseAssignment();
    }

    throwError("Parser: invalid statement");
    return nullptr;
}

auto Parser::parseFunctionDeclaration() -> std::unique_ptr<FunctionDeclaration> {
    // func is consumed, so can only be
    // - identifier {  # params and return type inferred (void)
    // - (             # params and return type explicitly defined

    std::vector<FunctionDeclaration::Parameter> parameters;
    std::string returnType = "void";

    if (match(TokenType::Symbol, "(")) {
        //  - ( type identifier, ... ) -> return_type # params and return type explicitly defined
        advance(); // consume "("

        while (!match(TokenType::Symbol, ")")) {
            std::string type = peek().getValue();
            consume(TokenType::Identifier);

            std::string name = peek().getValue();
            consume(TokenType::Identifier);

            parameters.emplace_back(type, name);

            if (match(TokenType::Symbol, ",")) {
                advance(); // consume ","
            }
        }
        consume(TokenType::Symbol, ")");
        consume(TokenType::Symbol, "->");
        std::string type = peek().getValue();
        consume(TokenType::Identifier);
    }

    if (match(TokenType::Keyword, "func")) {
        advance(); // consume "func" keyword
    }
    // from here it's
    // name { ... }
    std::string name = peek().getValue();
    consume(TokenType::Identifier);
    consume(TokenType::Symbol, "{");

    // parse function body

    consume(TokenType::Symbol, "}");

    return std::make_unique<FunctionDeclaration>(name, parameters, nullptr);
}

auto Parser::parseAssignment() -> std::unique_ptr<ASTNode> { return nullptr; }
