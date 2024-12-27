#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#pragma once
#include <iostream>

// Abstract syntax tree node forward declarations
class ASTNode;

class Program;
class Block;

// Statement nodes
class FunctionDeclaration;
class VariableDeclaration;

// Identifier nodes
class Literal;
class Reference;

// Operation nodes
class BinaryOperation;
class UnaryOperation;

// control flow nodes
class IfStatement;
class WhileLoop;
class ReturnStatement;

// memory management nodes
class MemoryAllocation;
class MemoryDeallocation;
class PointerAccess;
class PointerAssignment;

class ExpressionStatement;

// Class representing an abstract syntax tree node
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print(std::string indent) const = 0;
};

// Block node, representing a sequence of statements
class Block final : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    explicit Block(std::vector<std::unique_ptr<ASTNode>> statements) : statements(std::move(statements)) {}

    void print(std::string indent) const override;
};

// Program node, representing the entry point of the program
class Program final : public ASTNode {
public:
    std::string name;
    std::unique_ptr<Block> body;

    explicit Program(std::string name, std::unique_ptr<Block> body) : name(std::move(name)), body(std::move(body)) {}

    void print(std::string indent) const override;
};

// Function declaration node
class FunctionDeclaration final : public ASTNode {
public:
    class Parameter {
    public:
        std::string type;
        std::string name;

        Parameter(std::string type, std::string name) : type(std::move(type)), name(std::move(name)) {}
    };

    std::string name;
    std::vector<Parameter> parameters;
    std::unique_ptr<Block> body;

    FunctionDeclaration(std::string name, std::vector<Parameter> parameters, std::unique_ptr<Block> body) :
        name(std::move(name)), parameters(std::move(parameters)), body(std::move(body)) {}

    void print(std::string indent) const override;
};

// Function call node
class FunctionCall final : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> arguments;

    FunctionCall(std::string name, std::vector<std::unique_ptr<ASTNode>> arguments) :
        name(std::move(name)), arguments(std::move(arguments)) {}

    void print(std::string indent) const override;
};

// Variable declaration node
class VariableDeclaration final : public ASTNode {
public:
    std::string type;
    std::string name;
    std::unique_ptr<ASTNode> initializer;

    VariableDeclaration(std::string type, std::string name, std::unique_ptr<ASTNode> initializer) :
        type(std::move(type)), name(std::move(name)), initializer(std::move(initializer)) {}

    void print(std::string indent) const override;
};

// Literal node (e.g., numbers, strings)
class Literal final : public ASTNode {
public:
    std::string value;
    std::string type;

    explicit Literal(std::string value, std::string type) : value(std::move(value)), type(std::move(type)) {}

    void print(std::string indent) const override;
};

// Reference node (e.g., variable names, function names)
class Reference final : public ASTNode {
public:
    std::string name;

    explicit Reference(std::string name) : name(std::move(name)) {}

    void print(std::string indent) const override;
};

// Binary operation node (e.g., a + b)
class BinaryOperation final : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string operatorSymbol;
    std::unique_ptr<ASTNode> right;

    BinaryOperation(std::unique_ptr<ASTNode> left, std::string operatorSymbol, std::unique_ptr<ASTNode> right) :
        left(std::move(left)), operatorSymbol(std::move(operatorSymbol)), right(std::move(right)) {}

    void print(std::string indent) const override;
};

// Unary operation node (e.g., -a)
class UnaryOperation final : public ASTNode {
public:
    std::unique_ptr<ASTNode> operand;
    std::string operatorSymbol;

    UnaryOperation(std::unique_ptr<ASTNode> operand, std::string operatorSymbol) :
        operand(std::move(operand)), operatorSymbol(std::move(operatorSymbol)) {}

    void print(std::string indent) const override;
};

// Logical operation node (e.g., a && b)
class LogicalOperation final : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string operatorSymbol;
    std::unique_ptr<ASTNode> right;

    LogicalOperation(std::unique_ptr<ASTNode> left, std::string operatorSymbol, std::unique_ptr<ASTNode> right) :
        left(std::move(left)), operatorSymbol(std::move(operatorSymbol)), right(std::move(right)) {}

    void print(std::string indent) const override;
};

// If statement node
class IfStatement final : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Block> thenBranch;
    std::unique_ptr<Block> elseBranch;

    IfStatement(std::unique_ptr<ASTNode> condition, std::unique_ptr<Block> thenBranch,
                std::unique_ptr<Block> elseBranch = nullptr) :
        condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    void print(std::string indent) const override;
};

// While loop node
class WhileLoop final : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Block> body;

    WhileLoop(std::unique_ptr<ASTNode> condition, std::unique_ptr<Block> body) :
        condition(std::move(condition)), body(std::move(body)) {}

    void print(std::string indent) const override;
};

// Return statement node
class ReturnStatement final : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    explicit ReturnStatement(std::unique_ptr<ASTNode> expression) : expression(std::move(expression)) {}

    void print(std::string indent) const override;
};

// Expression statement node (e.g., standalone expressions in a block)
class ExpressionStatement final : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    explicit ExpressionStatement(std::unique_ptr<ASTNode> expression) : expression(std::move(expression)) {}

    void print(std::string indent) const override;
};

// Assignment node
class Assignment final : public ASTNode {
public:
    std::string name;
    std::unique_ptr<ASTNode> value;

    Assignment(std::string name, std::unique_ptr<ASTNode> value) : name(std::move(name)), value(std::move(value)) {}

    void print(std::string indent) const override;
};

// Memory allocation node
class MemoryAllocation final : public ASTNode {
public:
    std::string type;
    std::unique_ptr<ASTNode> sizeExpression;

    MemoryAllocation(std::string type, std::unique_ptr<ASTNode> sizeExpression) :
        type(std::move(type)), sizeExpression(std::move(sizeExpression)) {}

    void print(std::string indent) const override;
};

// Memory deallocation node
class MemoryDeallocation final : public ASTNode {
public:
    std::unique_ptr<ASTNode> pointer;

    explicit MemoryDeallocation(std::unique_ptr<ASTNode> pointer) : pointer(std::move(pointer)) {}

    void print(std::string indent) const override;
};

// Pointer access node
class PointerAccess final : public ASTNode {
public:
    std::unique_ptr<ASTNode> pointer;

    explicit PointerAccess(std::unique_ptr<ASTNode> pointer) : pointer(std::move(pointer)) {}

    void print(std::string indent) const override;
};

// Pointer assignment node
class PointerAssignment final : public ASTNode {
public:
    std::unique_ptr<ASTNode> pointer;
    std::unique_ptr<ASTNode> value;

    PointerAssignment(std::unique_ptr<ASTNode> pointer, std::unique_ptr<ASTNode> value) :
        pointer(std::move(pointer)), value(std::move(value)) {}

    void print(std::string indent) const override;
};
