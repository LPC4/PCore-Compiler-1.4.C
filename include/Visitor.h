#pragma once

class Block;
class Program;
class FunctionDeclaration;
class FunctionCall;
class VariableDeclaration;
class Literal;
class Reference;
class BinaryOperation;
class UnaryOperation;
class IfStatement;
class WhileLoop;
class ReturnStatement;
class ExpressionStatement;
class Assignment;
class MemoryAllocation;
class MemoryDeallocation;
class PointerAccess;
class PointerAssignment;

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(const Block &node) = 0;
    virtual void visit(const Program &node) = 0;
    virtual void visit(const FunctionDeclaration &node) = 0;
    virtual void visit(const FunctionCall &node) = 0;
    virtual void visit(const VariableDeclaration &node) = 0;
    virtual void visit(const Literal &node) = 0;
    virtual void visit(const Reference &node) = 0;
    virtual void visit(const BinaryOperation &node) = 0;
    virtual void visit(const UnaryOperation &node) = 0;
    virtual void visit(const IfStatement &node) = 0;
    virtual void visit(const WhileLoop &node) = 0;
    virtual void visit(const ReturnStatement &node) = 0;
    virtual void visit(const ExpressionStatement &node) = 0;
    virtual void visit(const Assignment &node) = 0;
    virtual void visit(const MemoryAllocation &node) = 0;
    virtual void visit(const MemoryDeallocation &node) = 0;
    virtual void visit(const PointerAccess &node) = 0;
    virtual void visit(const PointerAssignment &node) = 0;
};
