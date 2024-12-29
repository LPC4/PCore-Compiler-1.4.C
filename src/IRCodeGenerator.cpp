#include "../include/IRCodeGenerator.h"

void IRCodeGenerator::generateCode(const std::unique_ptr<Program> &program) {
    m_irCode.clear();
    m_registerCounter = 0;
    m_labelCounter = 0;
    program->accept(*this);

    printf("%s\n", m_irCode.c_str());
}

// Utility to get a new temporary register
std::string IRCodeGenerator::getTempRegister() {
    return "%t" + std::to_string(m_registerCounter++);
}

// Visitor functions
void IRCodeGenerator::visit(const Block &node) {
    for (const auto &statement : node.statements) {
        statement->accept(*this);
    }
}

void IRCodeGenerator::visit(const Program &node) {
    m_irCode += "; Program: " + node.name + "\n";
    node.body->accept(*this);
}

void IRCodeGenerator::visit(const FunctionDeclaration &node) {
    m_irCode += "; FunctionDeclaration: " + node.name + "\n";
    m_irCode += node.name + ":\n";
    node.body->accept(*this);
}

void IRCodeGenerator::visit(const VariableDeclaration &node) {
    m_irCode += "; VariableDeclaration: " + node.name + "\n";
    if (node.initializer) {
        node.initializer->accept(*this);
        m_irCode += "STORE [SP], " + getTempRegister() + "\n"; // Store the value on the stack
    }
    m_irCode += "PUSH\n"; // Allocate space on the stack
}

void IRCodeGenerator::visit(const FunctionCall &node) {
    m_irCode += "; FunctionCall: " + node.name + "\n";
    for (const auto &arg : node.arguments) {
        arg->accept(*this);
        m_irCode += "PUSH\n"; // Push each argument onto the stack
    }
    m_irCode += "CALL " + node.name + "\n";
    m_irCode += "POP %t0\n"; // Retrieve the return value into %t0
}

void IRCodeGenerator::visit(const Literal &node) {
    std::string tempReg = getTempRegister();
    m_irCode += "LOAD " + tempReg + ", " + node.value + "\n";
}

void IRCodeGenerator::visit(const Reference &node) {
    std::string tempReg = getTempRegister();
    m_irCode += "LOAD " + tempReg + ", [SP + " + std::to_string(node.offset) + "]\n";
}

void IRCodeGenerator::visit(const BinaryOperation &node) {
    node.left->accept(*this);
    std::string leftReg = getTempRegister();
    m_irCode += "POP " + leftReg + "\n";

    node.right->accept(*this);
    std::string rightReg = getTempRegister();
    m_irCode += "POP " + rightReg + "\n";

    std::string resultReg = getTempRegister();
    if (node.operatorSymbol == "+") {
        m_irCode += "ADD " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == "-") {
        m_irCode += "SUB " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == "*") {
        m_irCode += "MUL " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == "/") {
        m_irCode += "DIV " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == "||") {
        m_irCode += "OR " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == "&&") {
        m_irCode += "AND " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == "==") {
        m_irCode += "EQ " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == "!=") {
        m_irCode += "NE " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == "<") {
        m_irCode += "LT " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == "<=") {
        m_irCode += "LE " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == ">") {
        m_irCode += "GT " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else if (node.operatorSymbol == ">=") {
        m_irCode += "GE " + resultReg + ", " + leftReg + ", " + rightReg + "\n";
    } else {
        throw std::runtime_error("Unsupported binary operator: " + node.operatorSymbol);
    }

    m_irCode += "PUSH " + resultReg + "\n"; // Push the result back onto the stack
}

void IRCodeGenerator::visit(const UnaryOperation &node) {
    node.operand->accept(*this);
    std::string operandReg = getTempRegister();
    m_irCode += "POP " + operandReg + "\n";

    std::string resultReg = getTempRegister();
    if (node.operatorSymbol == "-") {
        m_irCode += "NEG " + resultReg + ", " + operandReg + "\n";
    } else if (node.operatorSymbol == "!") {
        m_irCode += "NOT " + resultReg + ", " + operandReg + "\n";
    }
    else {
        throw std::runtime_error("Unsupported unary operator: " + node.operatorSymbol);
    }

    m_irCode += "PUSH " + resultReg + "\n"; // Push the result back onto the stack
}

void IRCodeGenerator::visit(const IfStatement &node) {
    m_irCode += "; IfStatement\n";
    node.condition->accept(*this);
    m_irCode += "POP %t0\n"; // Evaluate condition into %t0
    std::string labelElse = "LABEL_ELSE_" + std::to_string(m_labelCounter++);
    std::string labelEnd = "LABEL_END_" + std::to_string(m_labelCounter++);
    m_irCode += "JZ %t0, " + labelElse + "\n"; // Jump if condition is false
    node.thenBranch->accept(*this);
    m_irCode += "JMP " + labelEnd + "\n";
    m_irCode += labelElse + ":\n";
    if (node.elseBranch) {
        node.elseBranch->accept(*this);
    }
    m_irCode += labelEnd + ":\n";
}

void IRCodeGenerator::visit(const WhileLoop &node) {
    m_irCode += "; WhileLoop\n";
    std::string labelStart = "LABEL_START_" + std::to_string(m_labelCounter++);
    std::string labelEnd = "LABEL_END_" + std::to_string(m_labelCounter++);
    m_irCode += labelStart + ":\n";
    node.condition->accept(*this);
    m_irCode += "POP %t0\n";
    m_irCode += "JZ %t0, " + labelEnd + "\n";
    node.body->accept(*this);
    m_irCode += "JMP " + labelStart + "\n";
    m_irCode += labelEnd + ":\n";
}

void IRCodeGenerator::visit(const ReturnStatement &node) {
    if (node.expression) {
        node.expression->accept(*this);
        m_irCode += "POP %t0\n"; // Return value in %t0
    }
    m_irCode += "RET\n";
}

void IRCodeGenerator::visit(const ExpressionStatement &node) {
    node.expression->accept(*this);
    m_irCode += "POP %t0\n"; // Discard the result
}

void IRCodeGenerator::visit(const Assignment &node) {
    node.value->accept(*this);
    m_irCode += "POP [SP + " + std::to_string(node.offset) + "]\n";
}

// dont need to implement these for now
void IRCodeGenerator::visit(const MemoryAllocation &node) {}
void IRCodeGenerator::visit(const MemoryDeallocation &node) {}
void IRCodeGenerator::visit(const PointerAccess &node) {}
void IRCodeGenerator::visit(const PointerAssignment &node) {}
