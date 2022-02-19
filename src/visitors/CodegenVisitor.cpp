#include <sstream>
#include <string>
#include "CodegenVisitor.h"
#include "../Token.h"
#include "../Exception.h"
#include "../ast/ExpressionAST.h"
#include "../ast/ModuleAST.h"
#include "../ast/NumberAST.h"
#include "../ast/VariableAST.h"
#include "../ast/FunctionAST.h"
#include "../ast/CompoundAST.h"
#include "../ast/PrototypeAST.h"
#include "../ast/ReturnAST.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Verifier.h"

CodegenVisitor::CodegenVisitor(const std::string& fname, std::unique_ptr<ModuleAST> ast) 
    : ast(std::move(ast)),
      ctx(std::make_unique<llvm::LLVMContext>()),
      builder(std::make_unique<llvm::IRBuilder<>>(*ctx)) {}

llvm::Value* CodegenVisitor::codegen() {
    return ast->accept(*this);
}

llvm::Value* CodegenVisitor::codegen(ExpressionAST* ast) {
    llvm::Value* L = ast->LHS->accept(*this);
    llvm::Value* R = ast->RHS->accept(*this);
    if (!L || !R)
    {
        return nullptr;
    }

    switch (ast->op) {
        case Operator::OP_ADD:
            return builder->CreateFAdd(L, R, "addtmp");
        case Operator::OP_SUB:
            return builder->CreateFSub(L, R, "subtmp");
        case Operator::OP_MUL:
            return builder->CreateFMul(L, R, "multmp");
        default:
            throw SyntaxError("invalid binary operator", SourceLocation(0,0));
    }
}

llvm::Value* CodegenVisitor::codegen(ModuleAST* ast) {
    for (auto& child : ast->children)
    {
        child->accept(*this);
    }
    return nullptr;
}

llvm::Value* CodegenVisitor::codegen(NumberAST* ast) {
    return llvm::ConstantFP::get(*ctx, llvm::APFloat(ast->val));
}

llvm::Value* CodegenVisitor::codegen(VariableAST* ast) {
    llvm::Value* val = namedValues[ast->id];
    if (!val)
    {
        std::ostringstream s;
        s << "unknown variable name '" << ast->id << '\'';
        throw ReferenceError(s.str(), SourceLocation(0,0));
    }
    return val;
}

llvm::Function* CodegenVisitor::codegen(FunctionAST* ast) {
    llvm::Function* func = mainModule->getFunction(ast->proto->name);
    if (!func)
    {
        func = (llvm::Function*)ast->proto->accept(*this);
        if (!func)
        {
            // if still nullptr then return nullptr
            return nullptr;
        }
    }
    if (!func->empty())
    {
        throw ReferenceError("cannot redefine function", SourceLocation(0,0));
    }
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*ctx, "entry", func);
    builder->SetInsertPoint(BB);

    namedValues.clear();
    for (auto& param : func->args())
    {
        namedValues[param.getName().str()] = &param;
    }
    
    if (llvm::Value* retVal = ast->body->accept(*this)) {
      builder->CreateRet(retVal);
      llvm::verifyFunction(*func);
      return func;
    }

    func->eraseFromParent();
    return nullptr;
}

llvm::Value* CodegenVisitor::codegen(CompoundAST* ast) {
    return nullptr;
}

llvm::Function* CodegenVisitor::codegen(PrototypeAST* ast) {
    std::vector<llvm::Type*> doubles(ast->params.size(), llvm::Type::getDoubleTy(*ctx));
    llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getDoubleTy(*ctx), doubles, false);
    llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, ast->name, mainModule.get());

    unsigned i = 0;
    for (auto& param : func->args())
    {
        param.setName(ast->params[i++]->id);
    }

    return func;
}

llvm::Value* CodegenVisitor::codegen(ReturnAST* ast) {
    return nullptr;
}
