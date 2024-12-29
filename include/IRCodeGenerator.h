#pragma once
#include <memory>
#include "AbstractSyntaxTree.h"
#include "Visitor.h"

class IRCodeGenerator final : public Visitor {
public:
    IRCodeGenerator() = default;

    void generateCode(const std::unique_ptr<Program> &program);

    // Visitor functions
    void visit(const Block &node) override;
    void visit(const Program &node) override;
    void visit(const FunctionDeclaration &node) override;
    void visit(const FunctionCall &node) override;
    void visit(const VariableDeclaration &node) override;
    void visit(const Literal &node) override;
    void visit(const Reference &node) override;
    void visit(const BinaryOperation &node) override;
    void visit(const UnaryOperation &node) override;
    void visit(const IfStatement &node) override;
    void visit(const WhileLoop &node) override;
    void visit(const ReturnStatement &node) override;
    void visit(const ExpressionStatement &node) override;
    void visit(const Assignment &node) override;
    void visit(const MemoryAllocation &node) override;
    void visit(const MemoryDeallocation &node) override;
    void visit(const PointerAccess &node) override;
    void visit(const PointerAssignment &node) override;

    auto getTempRegister() -> std::string;

private:
    std::string  m_irCode;
    unsigned int m_labelCounter;
    unsigned int m_registerCounter;
};
