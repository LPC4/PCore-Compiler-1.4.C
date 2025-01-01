#include <print>

#include "../include/CodeGenerator.h"

#include <set>

using namespace llvm;

CodeGenerator::CodeGenerator() : builder(context) {}

void CodeGenerator::generateCode(const std::unique_ptr<Program> &program) {
    program->accept(*this); // Start the code generation process

    // error handling + writing to file
    std::error_code errorCode;
    raw_fd_ostream  file("../output.ll", errorCode, sys::fs::OF_None);

    if (errorCode) {
        errs() << "Could not open file: " << errorCode.message() << "\n";
        return;
    }

    // Print the module to the file and standard output
    module->print(file, nullptr);
    module->print(outs(), nullptr);
}

void CodeGenerator::visit(Program &node) {
    module = std::make_unique<Module>(node.name, context);

    node.body->accept(*this);
}

void CodeGenerator::visit(Block &node) {
    for (const auto &statement : node.statements) {
        statement->accept(*this);
    }
}

void CodeGenerator::visit(FunctionDeclaration &node) {
    Type *returnType = typeToLLVMType(node.returnType);

    std::vector<Type *> paramTypes;
    for (const auto &param : node.parameters) {
        paramTypes.push_back(typeToLLVMType(param.type));
    }

    FunctionType *functionType = FunctionType::get(returnType, paramTypes, false);
    Function     *function = Function::Create(functionType, Function::ExternalLinkage, node.name, module.get());

    // Set the names for the function parameters
    unsigned idx = 0;
    for (auto &arg : function->args()) {
        arg.setName(node.parameters[idx++].name);
    }

    // Create a basic block to start insertion into
    BasicBlock *basicBlock = BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(basicBlock);

    // Allocate space for function parameters and store their values
    for (auto &arg : function->args()) {
        AllocaInst *alloca = builder.CreateAlloca(arg.getType(), nullptr, arg.getName() + ".addr");
        builder.CreateStore(&arg, alloca);

        refNameToValue[arg.getName().str()] = alloca;
        refNameToType[arg.getName().str()] = arg.getType();
    }

    if (node.body) {
        node.body->accept(*this);
    }
}

void CodeGenerator::visit(VariableDeclaration &node) {
    Type *varType = typeToLLVMType(node.type);
    if (varType == nullptr) {
        errs() << "Unknown type: " << node.type << "\n";
        return;
    }

    Function   *function = builder.GetInsertBlock()->getParent();
    IRBuilder   tmpBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());
    AllocaInst *alloca = tmpBuilder.CreateAlloca(varType, nullptr, node.name);

    refNameToValue[node.name] = alloca;
    refNameToType[node.name] = varType;

    if (node.initializer) {
        node.initializer->accept(*this);
        Value *value = node.initializer->getValue();

        if (value->getType()->isPointerTy()) {
            value = builder.CreateLoad(node.initializer->getType(), value, node.name);
        }

        if (value == nullptr) {
            errs() << "Failed to generate initializer for variable: " << node.name << "\n";
        }

        value = implicitConvert(value, varType, node.name);

        builder.CreateStore(value, alloca);
    }
}

static const std::set<std::string> BUILT_IN_FUNCTIONS = {"printf"};

void CodeGenerator::visit(FunctionCall &node) {
    if (BUILT_IN_FUNCTIONS.contains(node.name)) {
        // Handle built-in functions
        if (node.name == "printf") {
            // Handle printf
            std::vector<Value *> args;
            for (const auto &arg : node.arguments) {
                arg->accept(*this);
                Value *argValue = arg->getValue();

                if (argValue->getType()->isPointerTy()) {
                    argValue = builder.CreateLoad(arg->getType(), argValue, "loadArgTmp");
                }

                if (argValue != nullptr) {
                    args.push_back(argValue);
                } else {
                    errs() << "Failed to generate argument for printf\n";
                    return;
                }
            }

            FunctionCallee printfFunc = module->getOrInsertFunction(
                    "printf", FunctionType::get(IntegerType::getInt32Ty(context),
                                                PointerType::get(Type::getInt8Ty(context), 0), true));

            Value *result = builder.CreateCall(printfFunc, args, "printfResultTmp");
            node.setValue(result);
            node.setType(result->getType());
        }
        return;
    }

    Function *function = module->getFunction(node.name);
    if (function == nullptr) {
        errs() << "Function not found: " << node.name << "\n";
        return;
    }

    std::vector<Value *> args;
    for (const auto &arg : node.arguments) {
        arg->accept(*this);
        Value *argValue = arg->getValue();

        if (argValue->getType()->isPointerTy()) {
            argValue = builder.CreateLoad(arg->getType(), argValue, "loadArgTmp");
        }

        if (argValue != nullptr) {
            args.push_back(argValue);
        } else {
            errs() << "Failed to generate argument for function call: " << node.name << "\n";
            return;
        }
    }
    if (function->getReturnType()->isVoidTy()) {
        builder.CreateCall(function, args);
    } else {
        Value *value = builder.CreateCall(function, args, "callResultTmp");
        node.setValue(value);
        implicitConvert(value, function->getReturnType(), "callResultTmp");
        node.setType(value->getType());
    }
}

void CodeGenerator::visit(Literal &node) {
    // Generate the value for the literal, no pointer
    Value *value = getValueFromLiteral(node.value, node.type);

    node.setValue(value);
    node.setType(typeToLLVMType(node.type));
}

void CodeGenerator::visit(Reference &node) {
    // Generate code for the variable reference
    Value *value = refNameToValue[node.name];
    Type  *type = refNameToType[node.name];
    if (value == nullptr || type == nullptr) {
        errs() << "Unknown variable name: " << node.name << "\n";
    }
    node.setValue(value);
    node.setType(type);
}

void CodeGenerator::visit(BinaryOperation &node) {
    // Generate code for the left and right operands
    node.left->accept(*this);
    Value *leftValue = node.left->getValue();

    if (leftValue->getType()->isPointerTy()) {
        leftValue = builder.CreateLoad(node.left->getType(), leftValue, "loadLeftTmp");
    }

    node.right->accept(*this);
    Value *rightValue = node.right->getValue();

    if (rightValue->getType()->isPointerTy()) {
        rightValue = builder.CreateLoad(node.right->getType(), rightValue, "loadRightTmp");
    }

    Value *result = getBinaryLLVM(node.operatorSymbol, leftValue, rightValue);

    if (result == nullptr) {
        errs() << "Unknown binary operator: " << node.operatorSymbol << "\n";
        throw std::runtime_error("Unknown binary operator: " + node.operatorSymbol);
    }

    node.setValue(result);           // Store the generated value in the node
    node.setType(result->getType()); // Store the type in the node
}

void CodeGenerator::visit(UnaryOperation &node) {
    node.operand->accept(*this);
    Value *operandValue = node.operand->getValue();

    if (operandValue->getType()->isPointerTy()) {
        operandValue = builder.CreateLoad(node.operand->getType(), operandValue, "loadOperandTmp");
    }

    Value *result = getUnaryLLVM(node.operatorSymbol, operandValue);

    if (result == nullptr) {
        errs() << "Unknown unary operator: " << node.operatorSymbol << "\n";
        throw std::runtime_error("Unknown unary operator: " + node.operatorSymbol);
    }

    node.setValue(result);           // Store the generated value in the node
    node.setType(result->getType()); // Store the type in the node
}

void CodeGenerator::visit(IfStatement &node) {
    Function   *function = builder.GetInsertBlock()->getParent();
    BasicBlock *thenBlock = BasicBlock::Create(context, "then", function);
    BasicBlock *elseBlock = BasicBlock::Create(context, "else", function);
    BasicBlock *mergeBlock = BasicBlock::Create(context, "ifCont", function);

    node.condition->accept(*this);
    Value *condValue = node.condition->getValue();

    builder.CreateCondBr(condValue, thenBlock, elseBlock);

    builder.SetInsertPoint(thenBlock);
        node.thenBranch->accept(*this);
    builder.CreateBr(mergeBlock);

    builder.SetInsertPoint(elseBlock);
        if (node.elseBranch) {
            node.elseBranch->accept(*this);
        }
    builder.CreateBr(mergeBlock);

    builder.SetInsertPoint(mergeBlock);
}

void CodeGenerator::visit(WhileLoop &node) {
    Function   *function = builder.GetInsertBlock()->getParent();
    BasicBlock *headerBlock = BasicBlock::Create(context, "loop", function);
    BasicBlock *bodyBlock = BasicBlock::Create(context, "loopBody", function);
    BasicBlock *exitBlock = BasicBlock::Create(context, "loopExit", function);

    builder.CreateBr(headerBlock);

    builder.SetInsertPoint(headerBlock);
        node.condition->accept(*this);
        Value *condValue = node.condition->getValue();
    builder.CreateCondBr(condValue, bodyBlock, exitBlock);

    builder.SetInsertPoint(bodyBlock);
        node.body->accept(*this);
    builder.CreateBr(headerBlock);

    builder.SetInsertPoint(exitBlock);
}

void CodeGenerator::visit(ReturnStatement &node) {
    // Generate code for the return expression
    if (node.expression != nullptr) {
        node.expression->accept(*this);
        Value *returnValuePointer = node.expression->getValue();

        // Load the return value if it is a pointer (literals are stored directly)
        Value *returnValue = returnValuePointer;
        if (returnValuePointer->getType()->isPointerTy()) {
            returnValue = builder.CreateLoad(node.expression->getType(), returnValuePointer, "loadTmp");
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
    Value *valuePointer = node.value->getValue();


    Value *value = valuePointer;
    // todo: check if the value is a pointer and dereference it
    if (valuePointer->getType()->isPointerTy() /* && node.isPointerDereference */) {
        value = builder.CreateLoad(node.value->getType(), valuePointer, "loadTmp");
    }

    Value *variable = refNameToValue[node.name];
    Type  *variableType = refNameToType[node.name];

    if (!variable || !variableType) {
        errs() << "Unknown variable name or type: " << node.name << "\n";
        return;
    }

    value = implicitConvert(value, variableType, node.name);
    if (!value) {
        errs() << "Failed to convert value for: " << node.name << "\n";
        return;
    }

    builder.CreateStore(value, variable);
}

auto CodeGenerator::typeToLLVMType(const std::string &type) -> Type * {
    // to lowercase
    std::string type2 = type;
    std::ranges::transform(type2, type2.begin(), ::tolower);

    if (type2 == "char" || type2 == "byte") {
        return Type::getInt8Ty(context);
    }
    if (type2 == "int" || type2 == "integer") {
        return Type::getInt32Ty(context);
    }
    if (type2 == "float") {
        return Type::getFloatTy(context);
    }
    if (type2 == "bit") {
        return Type::getInt1Ty(context);
    }
    if (type2 == "void") {
        return Type::getVoidTy(context);
    }

    // Handle unknown types
    errs() << "Unknown type: " << type << "\n";
    return nullptr;
}

auto CodeGenerator::getValueFromLiteral(const std::string &value, const std::string &type) -> Value * {
    Type *llvmType = typeToLLVMType(type);
    if (!llvmType) {
        errs() << "Unknown type: " << type << "\n";
        return nullptr;
    }

    if (llvmType->isIntegerTy()) {
        if (type == "char") {
            return ConstantInt::get(llvmType, static_cast<uint32_t>(value[0]));
        }
        return ConstantInt::get(llvmType, std::stoi(value));
    }
    if (llvmType->isFloatTy()) {
        return ConstantFP::get(context, APFloat(std::stof(value)));
    }
    if (llvmType->isDoubleTy()) {
        return ConstantFP::get(context, APFloat(std::stod(value)));
    }
    if (llvmType->isPointerTy()) {
        if (type == "string") {
            return builder.CreateGlobalStringPtr(value);
        }
        return ConstantPointerNull::get(static_cast<PointerType *>(llvmType));
    }
    if (llvmType->isVoidTy()) {
        return nullptr;
    }

    return nullptr;
}

auto CodeGenerator::getBinaryLLVM(const std::string &op, Value *leftValue, Value *rightValue) -> Value * {
    const bool isFloat = leftValue->getType()->isFloatingPointTy() || rightValue->getType()->isFloatingPointTy();

    if (op == "+")
        return isFloat ? builder.CreateFAdd(leftValue, rightValue, "addTmp")
                       : builder.CreateAdd(leftValue, rightValue, "addTmp");
    if (op == "-")
        return isFloat ? builder.CreateFSub(leftValue, rightValue, "subTmp")
                       : builder.CreateSub(leftValue, rightValue, "subTmp");
    if (op == "*")
        return isFloat ? builder.CreateFMul(leftValue, rightValue, "mulTmp")
                       : builder.CreateMul(leftValue, rightValue, "mulTmp");
    if (op == "/")
        return isFloat ? builder.CreateFDiv(leftValue, rightValue, "divTmp")
                       : builder.CreateSDiv(leftValue, rightValue, "divTmp");
    if (op == "==")
        return isFloat ? builder.CreateFCmpOEQ(leftValue, rightValue, "eqTmp")
                       : builder.CreateICmpEQ(leftValue, rightValue, "eqTmp");
    if (op == "!=")
        return isFloat ? builder.CreateFCmpONE(leftValue, rightValue, "neTmp")
                       : builder.CreateICmpNE(leftValue, rightValue, "neTmp");
    if (op == "<")
        return isFloat ? builder.CreateFCmpOLT(leftValue, rightValue, "ltTmp")
                       : builder.CreateICmpSLT(leftValue, rightValue, "ltTmp");
    if (op == "<=")
        return isFloat ? builder.CreateFCmpOLE(leftValue, rightValue, "leTmp")
                       : builder.CreateICmpSLE(leftValue, rightValue, "leTmp");
    if (op == ">")
        return isFloat ? builder.CreateFCmpOGT(leftValue, rightValue, "gtTmp")
                       : builder.CreateICmpSGT(leftValue, rightValue, "gtTmp");
    if (op == ">=")
        return isFloat ? builder.CreateFCmpOGE(leftValue, rightValue, "geTmp")
                       : builder.CreateICmpSGE(leftValue, rightValue, "geTmp");
    if (op == "%")
        return builder.CreateSRem(leftValue, rightValue, "modTmp");
    if (op == "&&")
        return builder.CreateAnd(leftValue, rightValue, "andTmp");
    if (op == "||")
        return builder.CreateOr(leftValue, rightValue, "orTmp");
    if (op == "<<")
        return builder.CreateShl(leftValue, rightValue, "shlTmp");
    if (op == ">>")
        return builder.CreateAShr(leftValue, rightValue, "ashrTmp");
    return nullptr;
}

auto CodeGenerator::getUnaryLLVM(const std::string &op, Value *value) -> Value * {
    if (op == "-") {
        if (value->getType()->isFloatingPointTy()) {
            return builder.CreateFNeg(value, "fnegTmp"); // Negate floating-point value
        }
        if (value->getType()->isIntegerTy()) {
            return builder.CreateNeg(value, "negTmp"); // Negate integer value
        }
        throw std::runtime_error("Unknown type for unary operator: " + op);
    }
    if (op == "!") {
        return builder.CreateNot(value, "notTmp"); // Logical NOT
    }
    return nullptr;
}

constexpr int NUM_SIZE_BIT = 32;

auto CodeGenerator::implicitConvert(Value *value, Type *targetType, const std::string &name) -> Value * {

    if (const Type *valueType = value->getType(); valueType != targetType) {
        if (valueType->isIntegerTy(NUM_SIZE_BIT) && targetType->isFloatTy()) {
            return builder.CreateSIToFP(value, targetType, name + ".castIntToFloatTmp"); // Convert 32-bit int to float
        }
        if (valueType->isFloatTy() && targetType->isIntegerTy(NUM_SIZE_BIT)) {
            return builder.CreateFPToSI(value, targetType, name + ".castFloatToIntTmp"); // Convert float to 32-bit int
        }

        errs() << "Unsupported type conversion: " << name << "\n";
        return nullptr;
    }
    return value;
}
