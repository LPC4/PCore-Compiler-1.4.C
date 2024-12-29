#include "../include/CodeGenerator.h"
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Value.h>

CodeGenerator::CodeGenerator() :
    context(),
    module(std::make_unique<llvm::Module>("Module", context)),
    builder(context) { }

void CodeGenerator::generateCode(const std::unique_ptr<Program> &program) {
    // Empty
}

void CodeGenerator::visit(const Block &node) {
    // Empty
}

void CodeGenerator::visit(const Program &node) {
    // Empty
}

void CodeGenerator::visit(const FunctionDeclaration &node) {
    // Empty
}

void CodeGenerator::visit(const VariableDeclaration &node) {
    // Empty
}

void CodeGenerator::visit(const FunctionCall &node) {
    // Empty
}

void CodeGenerator::visit(const Literal &node) {
    // Empty
}

void CodeGenerator::visit(const Reference &node) {
    // Empty
}

void CodeGenerator::visit(const BinaryOperation &node) {
    // Empty
}

void CodeGenerator::visit(const UnaryOperation &node) {
    // Empty
}

void CodeGenerator::visit(const IfStatement &node) {
    // Empty
}

void CodeGenerator::visit(const WhileLoop &node) {
    // Empty
}

void CodeGenerator::visit(const ReturnStatement &node) {
    // Empty
}

void CodeGenerator::visit(const ExpressionStatement &node) {
    // Empty
}

void CodeGenerator::visit(const Assignment &node) {
    // Empty
}

void CodeGenerator::visit(const MemoryAllocation &node) {
    // Empty
}

void CodeGenerator::visit(const MemoryDeallocation &node) {
    // Empty
}

void CodeGenerator::visit(const PointerAccess &node) {
    // Empty
}

void CodeGenerator::visit(const PointerAssignment &node) {
    // Empty
}

llvm::Type* CodeGenerator::typeToLLVMType(const std::string &string) {
    // Empty, return nullptr
    return nullptr;
}
