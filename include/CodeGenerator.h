#pragma once
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

#include "AbstractSyntaxTree.h"
#include "Visitor.h"

class CodeGenerator : public Visitor {
public:
    llvm::LLVMContext             context;
    std::unique_ptr<llvm::Module> module;
    llvm::IRBuilder<>             builder;
    std::unordered_map<std::string, llvm::Value *>
            refNameToValue; // Map of reference name to LLVM value (pointer or literal value)
    std::unordered_map<std::string, llvm::Type *> refNameToType;

    CodeGenerator();
    void generateCode(const std::unique_ptr<Program> &program);

    auto typeToLLVMType(const std::string &type) -> llvm::Type *;
    auto getValueFromLiteral(const std::string &value, const std::string &type) -> llvm::Value *;
    auto getBinaryLLVM(const std::string &op, llvm::Value *leftValue, llvm::Value *rightValue) -> llvm::Value *;
    auto getUnaryLLVM(const std::string &op, llvm::Value *value) -> llvm::Value *;
    auto implicitConvert(llvm::Value *value, llvm::Type *targetType, const std::string &name) -> llvm::Value *;

    // Visitor functions
    void visit(Block &node) override;
    void visit(Program &node) override;
    void visit(FunctionDeclaration &node) override;
    void visit(FunctionCall &node) override;
    void visit(VariableDeclaration &node) override;
    void visit(Literal &node) override;
    void visit(Reference &node) override;
    void visit(BinaryOperation &node) override;
    void visit(UnaryOperation &node) override;
    void visit(IfStatement &node) override;
    void visit(WhileLoop &node) override;
    void visit(ReturnStatement &node) override;
    void visit(ExpressionStatement &node) override;
    void visit(Assignment &node) override;
};
