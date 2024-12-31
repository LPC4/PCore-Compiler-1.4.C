#include "../include/CodeGenerator.h"

CodeGenerator::CodeGenerator() : builder(context) {}

void CodeGenerator::generateCode(const std::unique_ptr<Program> &program) {
    program->accept(*this);

    std::error_code      errorCode;
    llvm::raw_fd_ostream file("../output.ll", errorCode, llvm::sys::fs::OF_None);

    if (errorCode) {
        llvm::errs() << "Could not open file: " << errorCode.message() << "\n";
        return;
    }

    // Print the module to the file and standard output
    module->print(file, nullptr);
    module->print(llvm::outs(), nullptr);
}

void CodeGenerator::visit(Program &node) {
    module = std::make_unique<llvm::Module>(node.name, context);

    node.body->accept(*this);
}

void CodeGenerator::visit(Block &node) {
    for (const auto &statement : node.statements) {
        statement->accept(*this);
    }
}

void CodeGenerator::visit(FunctionDeclaration &node) {
    llvm::Type *returnType = typeToLLVMType(node.returnType);

    std::vector<llvm::Type *> paramTypes;
    for (const auto &param : node.parameters) {
        paramTypes.push_back(typeToLLVMType(param.type));
    }

    llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, paramTypes, false);
    llvm::Function     *function =
            llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, node.name, module.get());

    // Set the names for the function parameters
    unsigned idx = 0;
    for (auto &arg : function->args()) {
        arg.setName(node.parameters[idx++].name);
    }

    // Create a basic block to start insertion into
    llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(basicBlock);

    if (node.body) {
        node.body->accept(*this);
    }
}

void CodeGenerator::visit(VariableDeclaration &node) {
    llvm::Type *varType = typeToLLVMType(node.type);
    if (varType == nullptr) {
        llvm::errs() << "Unknown type: " << node.type << "\n";
        return;
    }

    llvm::Function   *function = builder.GetInsertBlock()->getParent();
    llvm::IRBuilder<> tmpBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());
    llvm::AllocaInst *alloca = tmpBuilder.CreateAlloca(varType, nullptr, node.name);
    namedValues[node.name] = alloca;
    namedTypes[node.name] = varType;

    if (node.initializer) {
        node.initializer->accept(*this);
        llvm::Value *value = node.initializer->getValue();

        if (value->getType()->isPointerTy()) {
            value = builder.CreateLoad(node.initializer->getType(), value, node.name);
        }

        if (value != nullptr) {
            builder.CreateStore(value, alloca);
        } else {
            llvm::errs() << "Failed to generate initializer for variable: " << node.name << "\n";
        }
    }
}

void CodeGenerator::visit(FunctionCall &node) {
    std::vector<llvm::Value *> args;
    for (const auto &arg : node.arguments) {
        arg->accept(*this);
        llvm::Value *argValue = arg->getValue();
        if (argValue) {
            args.push_back(argValue);
        } else {
            llvm::errs() << "Failed to generate argument for function call: " << node.name << "\n";
            return;
        }
    }

    llvm::Function *function = module->getFunction(node.name);
    if (!function) {
        llvm::errs() << "Function not found: " << node.name << "\n";
        return;
    }

    builder.CreateCall(function, args);
}

void CodeGenerator::visit(Literal &node) {
    llvm::Value *value = getValueFromLiteral(node.value, node.type);

    node.setValue(value);
    node.setType(typeToLLVMType(node.type));
}

void CodeGenerator::visit(Reference &node) {
    // Generate code for the variable reference
    llvm::Value *value = namedValues[node.name];
    llvm::Type  *type = namedTypes[node.name];
    if (!value || !type) {
        llvm::errs() << "Unknown variable name: " << node.name << "\n";
    }
    node.setValue(value); // Store the generated value in the node
    node.setType(type);       // Store the type in the node
}

void CodeGenerator::visit(BinaryOperation &node) {
    // Generate code for the left and right operands
    node.left->accept(*this);
    llvm::Value *leftValue = node.left->getValue();

    if (leftValue->getType()->isPointerTy()) {
        leftValue = builder.CreateLoad(node.left->getType(), leftValue, "loadLeftTmp");
    }

    node.right->accept(*this);
    llvm::Value *rightValue = node.right->getValue();

    if (rightValue->getType()->isPointerTy()) {
        rightValue = builder.CreateLoad(node.right->getType(), rightValue, "loadRightTmp");
    }

    // Generate code for the binary operation
    llvm::Value *result = nullptr;
    if (node.operatorSymbol == "+") {
        result = builder.CreateAdd(leftValue, rightValue, "addtmp");
    } else if (node.operatorSymbol == "-") {
        result = builder.CreateSub(leftValue, rightValue, "subtmp");
    } else if (node.operatorSymbol == "*") {
        result = builder.CreateMul(leftValue, rightValue, "multmp");
    } else if (node.operatorSymbol == "/") {
        result = builder.CreateSDiv(leftValue, rightValue, "divtmp");
    } else if (node.operatorSymbol == "==") {
        result = builder.CreateICmpEQ(leftValue, rightValue, "eqtmp");
    } else if (node.operatorSymbol == "!=") {
        result = builder.CreateICmpNE(leftValue, rightValue, "netmp");
    } else if (node.operatorSymbol == "<") {
        result = builder.CreateICmpSLT(leftValue, rightValue, "lttmp");
    } else if (node.operatorSymbol == "<=") {
        result = builder.CreateICmpSLE(leftValue, rightValue, "letmp");
    } else if (node.operatorSymbol == ">") {
        result = builder.CreateICmpSGT(leftValue, rightValue, "gttmp");
    } else if (node.operatorSymbol == ">=") {
        result = builder.CreateICmpSGE(leftValue, rightValue, "getmp");
    } else if (node.operatorSymbol == "%") {
        result = builder.CreateSRem(leftValue, rightValue, "modtmp");
    } else if (node.operatorSymbol == "&&") {
        result = builder.CreateAnd(leftValue, rightValue, "andtmp");
    } else if (node.operatorSymbol == "||") {
        result = builder.CreateOr(leftValue, rightValue, "ortmp");
    }

    if (result == nullptr) {
        llvm::errs() << "Unknown binary operator: " << node.operatorSymbol << "\n";
        throw std::runtime_error("Unknown binary operator: " + node.operatorSymbol);
    }

    node.setValue(result); // Store the generated value in the node
    node.setType(result->getType()); // Store the type in the node
}

void CodeGenerator::visit(UnaryOperation &node) {
    // Generate code for the operand
    node.operand->accept(*this);
    llvm::Value *operandValue = node.operand->getValue();

    // Generate code for the unary operation
    llvm::Value *result = nullptr;
    if (node.operatorSymbol == "-") {
        result = builder.CreateNeg(operandValue, "negtmp");
    }
    if (node.operatorSymbol == "!") {
        result = builder.CreateNot(operandValue, "nottmp");
    }
    node.setValue(result); // Store the generated value in the node
}

void CodeGenerator::visit(IfStatement &node) {
    // Generate code for the condition
    node.condition->accept(*this);
    llvm::Value *condValue = node.condition->getValue();

    // Create blocks for the then and else cases
    llvm::Function   *function = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(context, "then", function);
    llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(context, "else", function);
    llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(context, "ifcont", function);

    // Generate conditional branch instruction
    builder.CreateCondBr(condValue, thenBlock, elseBlock);

    // Generate code for the then block
    builder.SetInsertPoint(thenBlock);
    node.thenBranch->accept(*this);
    builder.CreateBr(mergeBlock);

    // Generate code for the else block
    builder.SetInsertPoint(elseBlock);
    if (node.elseBranch) {
        node.elseBranch->accept(*this);
    }
    builder.CreateBr(mergeBlock);

    // Generate code for the merge block
    builder.SetInsertPoint(mergeBlock);
}

void CodeGenerator::visit(WhileLoop &node) {
    // Create blocks for the loop header, body, and exit
    llvm::Function   *function = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *headerBlock = llvm::BasicBlock::Create(context, "loop", function);
    llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(context, "loopbody", function);
    llvm::BasicBlock *exitBlock = llvm::BasicBlock::Create(context, "loopexit", function);

    // Generate unconditional branch to the loop header
    builder.CreateBr(headerBlock);

    // Generate code for the loop header
    builder.SetInsertPoint(headerBlock);
    node.condition->accept(*this);
    llvm::Value *condValue = node.condition->getValue();
    builder.CreateCondBr(condValue, bodyBlock, exitBlock);

    // Generate code for the loop body
    builder.SetInsertPoint(bodyBlock);
    node.body->accept(*this);
    builder.CreateBr(headerBlock);

    // Generate code for the loop exit
    builder.SetInsertPoint(exitBlock);
}
void CodeGenerator::visit(ReturnStatement &node) {
    // Generate code for the return value
    if (node.expression) {
        node.expression->accept(*this);
        llvm::Value *returnValuePointer = node.expression->getValue();

        // Load the return value if it is a pointer
        llvm::Value *returnValue = returnValuePointer;
        if (returnValuePointer->getType()->isPointerTy()) {
            returnValue = builder.CreateLoad(node.expression->getType(), returnValuePointer, "loadtmp");
        }

        builder.CreateRet(returnValue);
    } else {
        builder.CreateRetVoid();
    }
}

void CodeGenerator::visit(ExpressionStatement &node) {
    // Generate code for the expression
    node.expression->accept(*this);
}

void CodeGenerator::visit(Assignment &node) {
    // Generate code for the value to be assigned
    node.value->accept(*this);
    llvm::Value *valuePointer = node.value->getValue();

    // Load the value from the pointer if it is a pointer
    llvm::Value *value = valuePointer;
    if (valuePointer->getType()->isPointerTy()) {
        value = builder.CreateLoad(node.value->getType(), valuePointer, "loadtmp");
    }

    // Generate code for the variable reference
    llvm::Value *variable = namedValues[node.name];
    if (!variable) {
        llvm::errs() << "Unknown variable name: " << node.name << "\n";
    }

    builder.CreateStore(value, variable);
}

void CodeGenerator::visit(MemoryAllocation &node) {
    // Empty
}

void CodeGenerator::visit(MemoryDeallocation &node) {
    // Empty
}

void CodeGenerator::visit(PointerAccess &node) {
    // Empty
}

void CodeGenerator::visit(PointerAssignment &node) {
    // Empty
}

auto CodeGenerator::typeToLLVMType(const std::string &type) -> llvm::Type * {
    // to lowercase
    std::string type2 = type;
    std::ranges::transform(type2, type2.begin(), ::tolower);

    if (type2 == "char") {
        return llvm::Type::getInt8Ty(context);
    }
    if (type2 == "integer" || type2 == "int") {
        return llvm::Type::getInt32Ty(context);
    }
    if (type2 == "double") {
        return llvm::Type::getDoubleTy(context);
    }
    if (type2 == "float") {
        return llvm::Type::getFloatTy(context);
    }
    if (type2 == "bit") {
        return llvm::Type::getInt1Ty(context);
    }
    if (type2 == "void") {
        return llvm::Type::getVoidTy(context);
    }

    // Handle unknown types
    llvm::errs() << "Unknown type: " << type << "\n";
    return nullptr;
}

auto CodeGenerator::getValueFromLiteral(const std::string &value, const std::string &type) -> llvm::Value * {
    llvm::Type *llvmType = typeToLLVMType(type);
    if (llvmType == nullptr) {
        llvm::errs() << "Unknown type: " << type << "\n";
        return nullptr;
    }
    if (llvmType->isIntegerTy()) {
        return llvm::ConstantInt::get(llvmType, std::stoi(value));
    }
    if (llvmType->isFloatTy()) {
        return llvm::ConstantFP::get(context, llvm::APFloat(std::stof(value)));
    }
    if (llvmType->isDoubleTy()) {
        return llvm::ConstantFP::get(context, llvm::APFloat(std::stod(value)));
    }
    if (llvmType->isPointerTy()) {
        return llvm::ConstantPointerNull::get(static_cast<llvm::PointerType *>(llvmType));
    }
    if (llvmType->isVoidTy()) {
        return nullptr;
    }

    return nullptr;
}
