#pragma once
#include <memory>
#include "AbstractSyntaxTree.h"
#include "Visitor.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

class CodeGenerator : public Visitor {
public:
    LLVMContext             context;
    std::unique_ptr<Module> module;
    IRBuilder<>             builder;

    CodeGenerator();
    void generateCode(const std::unique_ptr<Program> &program);

    Type *typeToLLVMType(const std::string &string);

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
};
