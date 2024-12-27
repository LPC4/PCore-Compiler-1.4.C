#include "../include/Parser.h"

#include <algorithm>

#include "../include/ast/AbstractSyntaxTree.h"

Parser::Parser() = default;

void Parser::parse(std::vector<Token> tokens) {
    m_tokens = std::move(tokens);
    parseProgram();
}

auto Parser::parseProgram() -> std::unique_ptr<Program> {
    // program name
    consume(TokenType::Keyword, "program");

    const std::string programName = peek().getValue();
    printf("Program name: %s\n", programName.c_str());

    consume(TokenType::Identifier);
    consume(";");

    // program body
    while (!isAtEnd()) {
        parseDeclaration();
    }

    return std::make_unique<Program>(programName, nullptr);
};

auto Parser::parseDeclaration() -> std::unique_ptr<AbstractNode> {
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
    if (match(TokenType::Identifier)) { // the type of the variable
        return parseVariableDeclaration();
    }

    throwError("Parser: invalid statement");
    return nullptr;
}

auto Parser::parseFunctionDeclaration() -> std::unique_ptr<FunctionDeclaration> {
    // func is consumed, so can only be
    // - identifier {  # params and return type inferred (void)
    // - (             # params and return type explicitly defined

    std::vector<FunctionDeclaration::Parameter> parameters;
    std::string                                 returnType = "void";

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


    // parse body
    std::unique_ptr<Block> body; // parse body
    while (!match(TokenType::Symbol, "}")) {
        body->statements.push_back(parseStatement());
    }

    consume(TokenType::Symbol, "}");

    return std::make_unique<FunctionDeclaration>(name, parameters, std::move(body));
}

auto Parser::parseStatement() -> std::unique_ptr<AbstractNode> {
    // parse statement, which could be
    // - Variable declaration (type identifier)
    // - Assignment (identifier = expression)
    // - Function call (identifier (arguments))
    // - Return statement (return expression)
    // - If statement (if condition { ... } else { ... })
    // - While loop (while condition { ... })
    // - Binary operation (expression operator expression)
    // ? Expression statement (expression)
    // ? Block ( { ... } )
    // the rest are handled by the respective functions

    if (match(TokenType::Keyword, "return")) {
        return parseReturnStatement();
    }
    if (match(TokenType::Keyword, "if")) {
        return parseIfStatement();
    }
    if (match(TokenType::Keyword, "while")) {
        return parseWhileLoop();
    }
    if (match(TokenType::Identifier) && peekNext().getValue() == "(") {
        return parseFunctionCall();
    }
    if (match(TokenType::Identifier) && peekNext().getValue() == "=") {
        return parseAssignment();
    }
    if (match(TokenType::Identifier)) {
        return parseVariableDeclaration();
    }
    if (isBinaryOperation(peekUntilEOL())) {
        return parseBinaryOperation();
    }

    throwError("Parser: invalid statement");
    return nullptr;
}

auto Parser::parseVariableDeclaration() -> std::unique_ptr<VariableDeclaration> {
    return nullptr;
}

auto Parser::parseFunctionCall() -> std::unique_ptr<FunctionCall> {
    return nullptr;
}

auto Parser::parseBinaryOperation() -> std::unique_ptr<BinaryOperation> {
    return nullptr;
}

auto Parser::parseAssignment() -> std::unique_ptr<Assignment> {
    return nullptr;
}

auto Parser::parseIfStatement() -> std::unique_ptr<IfStatement> {
    return nullptr;
}

auto Parser::parseWhileLoop() -> std::unique_ptr<WhileLoop> {
    return nullptr;
}

auto Parser::parseReturnStatement() -> std::unique_ptr<ReturnStatement> {
    return nullptr;
}


