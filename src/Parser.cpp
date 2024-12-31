#include "../include/Parser.h"

#include <algorithm>

#include "../include/AbstractSyntaxTree.h"

Parser::Parser() = default;

auto Parser::parse(std::vector<Token> tokens) -> std::unique_ptr<Program> {
    m_tokens = std::move(tokens);
    return parseProgram();
}

auto Parser::parseProgram() -> std::unique_ptr<Program> {
    // program name
    consume(TokenType::Keyword, "program");

    const std::string programName = peek().getValue();

    consume(TokenType::Identifier);
    consume(";");

    // program body
    std::unique_ptr<Block> body = std::make_unique<Block>();
    while (!isAtEnd()) {
        body->statements.push_back(parseDeclaration());
    }

    return std::make_unique<Program>(programName, std::move(body));
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
}

auto Parser::parseFunctionDeclaration() -> std::unique_ptr<FunctionDeclaration> {
    // func is consumed, so can only be
    // - identifier {  # params and return type inferred (void)
    // - (             # params and return type explicitly defined

    std::vector<FunctionDeclaration::Parameter> parameters;
    std::string                                 returnType = "void";

    if (match(TokenType::Symbol, "(")) {
        //  - ( type identifier, ... ) -> return_type   # params and return type explicitly defined

        advance(); // consume "("

        while (!match(TokenType::Symbol, ")")) {
            const std::string type = peek().getValue();
            consume(TokenType::Identifier);

            const std::string name = peek().getValue();
            consume(TokenType::Identifier);

            parameters.emplace_back(FunctionDeclaration::Parameter{type, name});

            if (match(TokenType::Symbol, ",")) {
                advance(); // consume ","
            }
        }
        consume(TokenType::Symbol, ")");
        consume(TokenType::Symbol, "->");
        returnType = peek().getValue();
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
    std::unique_ptr<Block> body = std::make_unique<Block>();
    while (!match(TokenType::Symbol, "}")) {
        body->statements.push_back(parseStatement());
    }

    consume(TokenType::Symbol, "}");

    return std::make_unique<FunctionDeclaration>(name, parameters, std::move(body), returnType);
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
    // identifier( ... )
    if (match(TokenType::Identifier) && peekNext().getValue() == "(") {
        return parseFunctionCall();
    }
    // [*]identifier = expression;
    if (match(TokenType::Identifier) && peekNext().getValue() == "=" ||
        match(TokenType::Symbol, "*") && peekNext().getType() == TokenType::Identifier) {
        return parseAssignment();
    }
    // type [*|&] identifier [= expression];
    if (match(TokenType::Identifier)) {
        return parseVariableDeclaration();
    }

    throwError("Parser: invalid statement");
}

auto Parser::parseVariableDeclaration() -> std::unique_ptr<VariableDeclaration> {
    // type [*|&] identifier [= expression];

    std::string type = peek().getValue();
    consume(TokenType::Identifier);

    bool isPointer = false;
    bool isReference = false;

    if (match(TokenType::Symbol, "*")) {
        isPointer = true;
        advance(); // Consume the '*'
    } else if (match(TokenType::Symbol, "&")) {
        isReference = true;
        advance(); // Consume the '&'
    }

    std::string name = peek().getValue();
    consume(TokenType::Identifier);

    std::unique_ptr<AbstractNode> initializer = nullptr;
    if (match(TokenType::Symbol, "=")) {
        advance(); // Consume the '='
        initializer = parseExpression();
    }

    consume(TokenType::Symbol, ";");

    return std::make_unique<VariableDeclaration>(type, name, isPointer, isReference, std::move(initializer));
}

auto Parser::parseExpression() -> std::unique_ptr<AbstractNode> {
    // Parse an expression by parsing a binary operation, which will recursively parse the expression
    return parseBinaryOperation();
}

auto Parser::parseBinaryOperation(const int precedence) -> std::unique_ptr<AbstractNode> {
    // Parse binary operations with precedence
    // - Parse left-hand side
    // - Parse operator
    // - Parse right-hand side

    auto lhs = parseUnaryExpression();

    while (true) {
        if (!isBinaryOperator(peek())) {
            break;
        }

        const int currentPrecedence = getOperatorPrecedence(peek().getValue());
        if (currentPrecedence < precedence) {
            break;
        }

        std::string op = peek().getValue();
        advance(); // Consume operator

        auto rhs = parseBinaryOperation(currentPrecedence + 1);

        lhs = std::make_unique<BinaryOperation>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
}

auto Parser::parseAssignment() -> std::unique_ptr<Assignment> {
    // [*]identifier = expression;

    bool isPointerDereference = false;
    if (match(TokenType::Symbol, "*")) {
        advance(); // Consume the '*'
        isPointerDereference = true;
    }

    std::string variable = peek().getValue();
    consume(TokenType::Identifier);

    consume(TokenType::Symbol, "=");

    auto value = parseExpression();

    consume(TokenType::Symbol, ";");

    return std::make_unique<Assignment>(variable, std::move(value), isPointerDereference);
}

auto Parser::parseFunctionCall() -> std::unique_ptr<FunctionCall> {
    // identifier([argument, ...])

    std::string functionName = peek().getValue();
    consume(TokenType::Identifier);
    consume(TokenType::Symbol, "(");

    std::vector<std::unique_ptr<AbstractNode>> arguments;
    if (!match(TokenType::Symbol, ")")) {
        while (true) {
            // todo: argument is not just any expression, its of type
            //  - type[*|&] identifier
            arguments.push_back(parseExpression());
            if (match(TokenType::Symbol, ",")) {
                advance(); // Consume ","
            } else {
                break;
            }
        }
    }

    consume(TokenType::Symbol, ")");

    return std::make_unique<FunctionCall>(functionName, std::move(arguments));
}

auto Parser::parseIfStatement() -> std::unique_ptr<IfStatement> {
    // if condition { ... } else { ... }

    consume(TokenType::Keyword, "if");

    auto condition = parseExpression();

    auto thenBranch = parseBlock();

    if (match(TokenType::Keyword, "else")) {
        advance(); // Consume "else"
        auto elseBranch = parseBlock();
        return std::make_unique<IfStatement>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
    }

    return std::make_unique<IfStatement>(std::move(condition), std::move(thenBranch));
}

auto Parser::parseWhileLoop() -> std::unique_ptr<WhileLoop> {
    // while condition { ... }

    consume(TokenType::Keyword, "while");

    auto condition = parseExpression();

    auto body = parseBlock();

    return std::make_unique<WhileLoop>(std::move(condition), std::move(body));
}

auto Parser::parseBlock() -> std::unique_ptr<Block> {
    // { ... }

    auto block = std::make_unique<Block>();

    consume(TokenType::Symbol, "{");

    while (!match(TokenType::Symbol, "}")) {
        block->statements.push_back(parseStatement());
    }

    consume(TokenType::Symbol, "}");

    return block;
}

auto Parser::parseReturnStatement() -> std::unique_ptr<ReturnStatement> {
    // return [expression];

    consume(TokenType::Keyword, "return");

    std::unique_ptr<AbstractNode> value = nullptr;
    if (!match(TokenType::Symbol, ";")) {
        value = parseExpression();
    }

    consume(TokenType::Symbol, ";");

    return std::make_unique<ReturnStatement>(std::move(value));
}

auto Parser::parsePrimaryExpression() -> std::unique_ptr<AbstractNode> {
    // Parse primary expressions, which can be
    // - Literal (integer, float, char, string)
    // - Reference ([&]identifier)
    // - Function call (identifier ([arguments]))
    // - Parenthesized expression ((expression))

    if (match(TokenType::Integer) || match(TokenType::Float) || match(TokenType::Char) || match(TokenType::String)) {
        auto value = peek().getValue();
        auto type = tokenTypeToString(peek().getType());
        advance();
        return std::make_unique<Literal>(value, type);
    }

    if (match(TokenType::Identifier)) {
        bool isReference = false;
        if (peek().getValue() == "&") {
            advance(); // Consume "&"
            isReference = true;
        }

        std::string name = peek().getValue();
        if (peekNext().getValue() == "(") {
            return parseFunctionCall(); // Handle function call
        }
        advance();

        return std::make_unique<Reference>(name, isReference); // Variable reference
    }

    if (match(TokenType::Symbol, "(")) {
        advance(); // Consume "("
        auto expr = parseExpression();
        consume(TokenType::Symbol, ")");
        return expr; // Parenthesized expression
    }

    throwError("Unexpected primary expression.");
}

auto Parser::parseUnaryExpression() -> std::unique_ptr<AbstractNode> {
    // Parse unary expressions, which can be
    // - -expr
    // - !expr

    if (match(TokenType::Symbol, "-") || match(TokenType::Symbol, "!")) {
        std::string op = peek().getValue();
        advance();                             // Consume the operator
        auto operand = parseUnaryExpression(); // Recursively parse the operand
        return std::make_unique<UnaryOperation>(std::move(operand), op);
    }
    return parsePrimaryExpression(); // If no unary operator, parse a primary expression
}
