#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

// Base class for all AST nodes
class ASTNode {};

// Block node, representing a sequence of statements
class Block : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    explicit Block(std::vector<std::unique_ptr<ASTNode>> statements)
        : statements(std::move(statements)) {}
};

// Program node, representing the entry point of the program
class Program : public ASTNode {
public:
    std::string name;
    std::unique_ptr<Block> body;

    explicit Program(std::string name, std::unique_ptr<Block> body)
        : name(std::move(name)), body(std::move(body)) {}
};

// Function declaration node
class FunctionDeclaration : public ASTNode {
public:
    class Parameter {
    public:
        std::string type;
        std::string name;

        Parameter(std::string type, std::string name)
            : type(std::move(type)), name(std::move(name)) {}
    };

    std::string name;
    std::vector<Parameter> parameters;
    std::unique_ptr<Block> body;

    FunctionDeclaration(std::string name, std::vector<Parameter> parameters, std::unique_ptr<Block> body)
        : name(std::move(name)), parameters(std::move(parameters)), body(std::move(body)) {}
};

// Variable declaration node
class VariableDeclaration : public ASTNode {
public:
    std::string type;
    std::string name;
    std::unique_ptr<ASTNode> initializer;

    VariableDeclaration(std::string type, std::string name, std::unique_ptr<ASTNode> initializer)
        : type(std::move(type)), name(std::move(name)), initializer(std::move(initializer)) {}
};

// Literal node (e.g., numbers, strings)
class Literal : public ASTNode {
public:
    std::string value;

    explicit Literal(std::string value) : value(std::move(value)) {}
};

// Binary operation node (e.g., a + b)
class BinaryOperation : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string operatorSymbol;
    std::unique_ptr<ASTNode> right;

    BinaryOperation(std::unique_ptr<ASTNode> left, std::string operatorSymbol, std::unique_ptr<ASTNode> right)
        : left(std::move(left)), operatorSymbol(std::move(operatorSymbol)), right(std::move(right)) {}
};

// If statement node
class IfStatement : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Block> thenBranch;
    std::unique_ptr<Block> elseBranch;

    IfStatement(std::unique_ptr<ASTNode> condition, std::unique_ptr<Block> thenBranch, std::unique_ptr<Block> elseBranch = nullptr)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
};

// While loop node
class WhileLoop : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Block> body;

    WhileLoop(std::unique_ptr<ASTNode> condition, std::unique_ptr<Block> body)
        : condition(std::move(condition)), body(std::move(body)) {}
};

// Return statement node
class ReturnStatement : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    explicit ReturnStatement(std::unique_ptr<ASTNode> expression)
        : expression(std::move(expression)) {}
};

// Expression statement node (e.g., standalone expressions in a block)
class ExpressionStatement : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    explicit ExpressionStatement(std::unique_ptr<ASTNode> expression)
        : expression(std::move(expression)) {}
};
