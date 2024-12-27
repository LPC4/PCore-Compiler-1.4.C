#include "../../include/ast/AbstractSyntaxTree.h"

void Block::print(const std::string indent) const {
    std::cout << indent << "Block" << '\n';
    for (const auto &statement : statements) {
        statement->print(indent + "  ");
    }
}

void Program::print(const std::string indent) const {
    std::cout << indent << "Program: " << name << '\n';
    body->print(indent + "  ");
}

void FunctionDeclaration::print(const std::string indent) const {
    std::cout << indent << "Function Declaration: " << name << '\n';
    for (const auto &parameter : parameters) {
        std::cout << indent + "  " << "Parameter: " << parameter.type << " " << parameter.name << '\n';
    }
    body->print(indent + "  ");
}

void FunctionCall::print(const std::string indent) const {
    std::cout << indent << "Function Call: " << name << '\n';
    for (const auto &argument : arguments) {
        argument->print(indent + "  ");
    }
}

void VariableDeclaration::print(const std::string indent) const {
    std::cout << indent << "Variable Declaration: " << type << " " << name << '\n';
    if (this->initializer) {
        this->initializer->print(indent + "  ");
    }
}

void Assignment::print(const std::string indent) const {
    std::cout << indent << "Assignment: " << name << '\n';
    value->print(indent + "  ");
}

void MemoryAllocation::print(const std::string indent) const {
    std::cout << indent << "Memory Allocation: " << type << '\n';
    sizeExpression->print(indent + "  ");
}

void MemoryDeallocation::print(const std::string indent) const {
    std::cout << indent << "Memory Deallocation: " << this->pointer << '\n';
}

void PointerAccess::print(const std::string indent) const {
    std::cout << indent << "Pointer Access" << '\n';
    pointer->print(indent + "  ");
}

void PointerAssignment::print(const std::string indent) const {
    std::cout << indent << "Pointer Assignment" << '\n';
    pointer->print(indent + "  ");
}


void ReturnStatement::print(const std::string indent) const {
    std::cout << indent << "Return Statement" << '\n';
    expression->print(indent + "  ");
}

void ExpressionStatement::print(const std::string indent) const {
    std::cout << indent << "Expression Statement" << '\n';
    expression->print(indent + "  ");
}

void WhileLoop::print(const std::string indent) const {
    std::cout << indent << "While Loop" << '\n';
    condition->print(indent + "  ");
    body->print(indent + "  ");
}

void IfStatement::print(const std::string indent) const {
    std::cout << indent << "If Statement" << '\n';
    condition->print(indent + "  ");
    thenBranch->print(indent + "  ");
    if (elseBranch) {
        elseBranch->print(indent + "  ");
    }
}

void Literal::print(const std::string indent) const { std::cout << indent << "Literal: " << value << '\n'; }

void BinaryOperation::print(const std::string indent) const {
    std::cout << indent << "Binary Operation: " << operatorSymbol << '\n';
    left->print(indent + "  ");
    right->print(indent + "  ");
}

void UnaryOperation::print(const std::string indent) const {
    std::cout << indent << "Unary Operation: " << operatorSymbol << '\n';
    operand->print(indent + "  ");
}

void LogicalOperation::print(const std::string indent) const {
    std::cout << indent << "Logical Operation: " << operatorSymbol << '\n';
    left->print(indent + "  ");
    right->print(indent + "  ");
}

void ParenthesizedExpression::print(const std::string indent) const {
    std::cout << indent << "Parenthesized Expression" << '\n';
    expression->print(indent + "  ");
}

void Reference::print(const std::string indent) const { std::cout << indent << "Reference: " << name << '\n'; }
