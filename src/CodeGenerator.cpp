#include "../include/CodeGenerator.h"

void CodeGenerator::generateCode(const std::unique_ptr<Program> &program) {
    program->accept(*this);
}


// Visitor functions
void CodeGenerator::visit(const Block& node) {
    // Handle Block node
    for (const auto& statement : node.statements) {
        statement->accept(*this);
    }
}

void CodeGenerator::visit(const Program& node) {
    // Handle Program node
    printf("Program: %s\n", node.name.c_str());
    node.body->accept(*this);
}

void CodeGenerator::visit(const FunctionDeclaration& node) {
    // Handle FunctionDeclaration node
    printf("FunctionDeclaration: %s\n", node.name.c_str());
    node.body->accept(*this);
}

void CodeGenerator::visit(const VariableDeclaration& node) {
    // Handle VariableDeclaration node
    printf("VariableDeclaration: %s\n", node.name.c_str());
}

void CodeGenerator::visit(const FunctionCall& node) {
    // Handle FunctionCall node
    printf("FunctionCall: %s\n", node.name.c_str());
}

void CodeGenerator::visit(const Literal& node) {
    // Handle Literal node
    printf("Literal: %s\n", node.value.c_str());
}

void CodeGenerator::visit(const Reference& node) {
    // Handle Reference node
    printf("Reference: %s\n", node.name.c_str());
}

void CodeGenerator::visit(const BinaryOperation& node) {
    // Handle BinaryOperation node
    printf("BinaryOperation: %s\n", node.operatorSymbol.c_str());
}

void CodeGenerator::visit(const UnaryOperation& node) {
    // Handle UnaryOperation node
    printf("UnaryOperation: %s\n", node.operatorSymbol.c_str());
}

void CodeGenerator::visit(const IfStatement& node) {
    // Handle IfStatement node
    printf("IfStatement\n");
}

void CodeGenerator::visit(const WhileLoop& node) {
    // Handle WhileLoop node
    printf("WhileLoop\n");
}

void CodeGenerator::visit(const ReturnStatement& node) {
    // Handle ReturnStatement node
    printf("ReturnStatement\n");
}

void CodeGenerator::visit(const ExpressionStatement& node) {
    // Handle ExpressionStatement node
    printf("ExpressionStatement\n");
}

void CodeGenerator::visit(const Assignment& node) {
    // Handle Assignment node
    printf("Assignment: %s\n", node.name.c_str());
}

void CodeGenerator::visit(const MemoryAllocation& node) {
    // Handle MemoryAllocation node
    printf("MemoryAllocation: %s\n", node.type.c_str());
}

void CodeGenerator::visit(const MemoryDeallocation& node) {
    // Handle MemoryDeallocation node
    printf("MemoryDeallocation\n");
}

void CodeGenerator::visit(const PointerAccess& node) {
    // Handle PointerAccess node
    printf("PointerAccess\n");
}

void CodeGenerator::visit(const PointerAssignment& node) {
    // Handle PointerAssignment node
    printf("PointerAssignment\n");
}

