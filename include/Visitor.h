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

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(Block &node) = 0;
    virtual void visit(Program &node) = 0;
    virtual void visit(FunctionDeclaration &node) = 0;
    virtual void visit(FunctionCall &node) = 0;
    virtual void visit(VariableDeclaration &node) = 0;
    virtual void visit(Literal &node) = 0;
    virtual void visit(Reference &node) = 0;
    virtual void visit(BinaryOperation &node) = 0;
    virtual void visit(UnaryOperation &node) = 0;
    virtual void visit(IfStatement &node) = 0;
    virtual void visit(WhileLoop &node) = 0;
    virtual void visit(ReturnStatement &node) = 0;
    virtual void visit(ExpressionStatement &node) = 0;
    virtual void visit(Assignment &node) = 0;
};
