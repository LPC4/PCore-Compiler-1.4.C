#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#pragma once
#include <iostream>

#include "Visitor.h"

namespace llvm {
class Value;
}
// Abstract syntax tree node forward declarations
class AbstractNode;
class Program;
class Block;

// Declarations
class FunctionDeclaration;
class VariableDeclaration;

// Identifier nodes
class Literal;
class Reference;

// Operation nodes
class BinaryOperation;
class UnaryOperation;

// statements
class IfStatement;
class WhileLoop;
class ReturnStatement;
class Assignment;
class ExpressionStatement;

// memory management nodes
class MemoryAllocation;
class MemoryDeallocation;
class PointerAccess;
class PointerAssignment;

// Class representing an abstract syntax tree node
class AbstractNode {
public:
    virtual ~AbstractNode() = default;
    AbstractNode() = default;
    AbstractNode(const AbstractNode &node) = default;
    AbstractNode(AbstractNode &&node) noexcept = default;
    AbstractNode &operator=(const AbstractNode &node) = default;
    AbstractNode &operator=(AbstractNode &&node) noexcept = default;

    virtual void print(std::string indent = "") const = 0;
    virtual void accept(Visitor &visitor) const = 0;

    // Set and get LLVM value methods
    void setLLVMValue(llvm::Value* value) { llvmValue = value; }
    llvm::Value* getLLVMValue() const { return llvmValue; }

private:
    llvm::Value* llvmValue = nullptr;  // Holds the LLVM value for this node
};

// Block node, representing a sequence of statements
class Block final : public AbstractNode {
public:
    std::vector<std::unique_ptr<AbstractNode>> statements;

    explicit Block(std::vector<std::unique_ptr<AbstractNode>> statements) : statements(std::move(statements)) {}
    Block() = default;

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Program node, representing the entry point of the program
class Program final : public AbstractNode {
public:
    std::string            name;
    std::unique_ptr<Block> body;

    explicit Program(std::string name, std::unique_ptr<Block> body) : name(std::move(name)), body(std::move(body)) {}
    Program() = default;

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Function declaration node
class FunctionDeclaration final : public AbstractNode {
public:
    class Parameter;

    std::string            name;
    std::vector<Parameter> parameters;
    std::unique_ptr<Block> body;
    std::string            returnType;

    FunctionDeclaration(std::string name, std::vector<Parameter> parameters, std::unique_ptr<Block> body,
                        std::string returnType) :
        name(std::move(name)), parameters(std::move(parameters)), body(std::move(body)),
        returnType(std::move(returnType)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }

    class Parameter {
    public:
        std::string type;
        std::string name;

        Parameter(std::string type, std::string name) : type(std::move(type)), name(std::move(name)) {}
    };
};

// Function call node
class FunctionCall final : public AbstractNode {
public:
    std::string                                name;
    std::vector<std::unique_ptr<AbstractNode>> arguments;

    FunctionCall(std::string name, std::vector<std::unique_ptr<AbstractNode>> arguments) :
        name(std::move(name)), arguments(std::move(arguments)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

class VariableDeclaration : public AbstractNode {
public:
    std::string                   type;
    std::string                   name;
    bool                          isPointer;
    bool                          isReference;
    std::unique_ptr<AbstractNode> initializer;

    VariableDeclaration(const std::string &type, const std::string &name, const bool isPointer, const bool isReference,
                        std::unique_ptr<AbstractNode> initializer) :
        type(type), name(name), isPointer(isPointer), isReference(isReference), initializer(std::move(initializer)) {}

    VariableDeclaration(std::string type, std::string name, std::unique_ptr<AbstractNode> initializer) :
        type(std::move(type)), name(std::move(name)), isPointer(false), isReference(false),
        initializer(std::move(initializer)) {}

    VariableDeclaration(std::string type, std::string name, std::unique_ptr<AbstractNode> initializer,
                        const bool isPointer) :
        type(std::move(type)), name(std::move(name)), isPointer(isPointer), isReference(false),
        initializer(std::move(initializer)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Literal node (e.g., numbers, strings)
class Literal final : public AbstractNode {
public:
    std::string value;
    std::string type;

    explicit Literal(std::string value, std::string type) : value(std::move(value)), type(std::move(type)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Reference node (e.g., variable names, function names)
class Reference final : public AbstractNode {
public:
    std::string name;
    bool        isReference;

    explicit Reference(std::string name, const bool isReference) : name(std::move(name)), isReference(isReference) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Binary operation node (e.g., a + b)
class BinaryOperation final : public AbstractNode {
public:
    std::unique_ptr<AbstractNode> left;
    std::string                   operatorSymbol;
    std::unique_ptr<AbstractNode> right;

    BinaryOperation(std::unique_ptr<AbstractNode> left, std::string operatorSymbol,
                    std::unique_ptr<AbstractNode> right) :
        left(std::move(left)), operatorSymbol(std::move(operatorSymbol)), right(std::move(right)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Unary operation node (e.g., -a)
class UnaryOperation final : public AbstractNode {
public:
    std::unique_ptr<AbstractNode> operand;
    std::string                   operatorSymbol;

    UnaryOperation(std::unique_ptr<AbstractNode> operand, std::string operatorSymbol) :
        operand(std::move(operand)), operatorSymbol(std::move(operatorSymbol)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// If statement node
class IfStatement final : public AbstractNode {
public:
    std::unique_ptr<AbstractNode> condition;
    std::unique_ptr<Block>        thenBranch;
    std::unique_ptr<Block>        elseBranch;

    IfStatement(std::unique_ptr<AbstractNode> condition, std::unique_ptr<Block> thenBranch,
                std::unique_ptr<Block> elseBranch = nullptr) :
        condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// While loop node
class WhileLoop final : public AbstractNode {
public:
    std::unique_ptr<AbstractNode> condition;
    std::unique_ptr<Block>        body;

    WhileLoop(std::unique_ptr<AbstractNode> condition, std::unique_ptr<Block> body) :
        condition(std::move(condition)), body(std::move(body)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Return statement node
class ReturnStatement final : public AbstractNode {
public:
    std::unique_ptr<AbstractNode> expression;

    explicit ReturnStatement(std::unique_ptr<AbstractNode> expression) : expression(std::move(expression)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Expression statement node (e.g., standalone expressions in a block)
class ExpressionStatement final : public AbstractNode {
public:
    std::unique_ptr<AbstractNode> expression;

    explicit ExpressionStatement(std::unique_ptr<AbstractNode> expression) : expression(std::move(expression)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Assignment node
class Assignment final : public AbstractNode {
public:
    std::string                   name;
    std::unique_ptr<AbstractNode> value;
    bool                          isPointerDereference;

    Assignment(std::string name, std::unique_ptr<AbstractNode> value, const bool isPointerDereference) :
        name(std::move(name)), value(std::move(value)), isPointerDereference(isPointerDereference) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Memory allocation node
class MemoryAllocation final : public AbstractNode {
public:
    std::string                   type;
    std::unique_ptr<AbstractNode> sizeExpression;

    MemoryAllocation(std::string type, std::unique_ptr<AbstractNode> sizeExpression) :
        type(std::move(type)), sizeExpression(std::move(sizeExpression)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Memory deallocation node
class MemoryDeallocation final : public AbstractNode {
public:
    std::unique_ptr<AbstractNode> pointer;

    explicit MemoryDeallocation(std::unique_ptr<AbstractNode> pointer) : pointer(std::move(pointer)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Pointer access node
class PointerAccess final : public AbstractNode {
public:
    std::unique_ptr<AbstractNode> pointer;

    explicit PointerAccess(std::unique_ptr<AbstractNode> pointer) : pointer(std::move(pointer)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};

// Pointer assignment node
class PointerAssignment final : public AbstractNode {
public:
    std::unique_ptr<AbstractNode> pointer;
    std::unique_ptr<AbstractNode> value;

    PointerAssignment(std::unique_ptr<AbstractNode> pointer, std::unique_ptr<AbstractNode> value) :
        pointer(std::move(pointer)), value(std::move(value)) {}

    void print(std::string indent) const override;
    void accept(Visitor &visitor) const override { visitor.visit(*this); }
};
