#include <cstddef>
#include <iostream>
#include <memory>
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
#include "../ast/CallAST.h"
#include "../ast/IfAST.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

CodegenVisitor::CodegenVisitor(const std::string& fname, std::unique_ptr<ModuleAST> ast) 
    : ast(std::move(ast)),
      ctx(std::make_unique<llvm::LLVMContext>()),
      builder(std::make_unique<llvm::IRBuilder<>>(*ctx)),
      mainModule(std::make_unique<llvm::Module>(fname, *ctx)),
      currFunc(nullptr) {}

void CodegenVisitor::print()
{
    mainModule->print(llvm::errs(), nullptr);
}

int CodegenVisitor::emitObjectCode()
{
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    
    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    if (!target) {
        llvm::errs() << error;
        return 1;
    }
    auto cpu = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, rm);
    mainModule->setDataLayout(targetMachine->createDataLayout());
    mainModule->setTargetTriple(targetTriple);
    auto filename = "output.o";
    std::error_code ec;
    llvm::raw_fd_ostream dest(filename, ec, llvm::sys::fs::OF_None);

    if (ec) {
        llvm::errs() << "Could not open file: " << ec.message();
        return 1;
    }
    
    llvm::legacy::PassManager pass;
    auto ftype = llvm::CGFT_ObjectFile;

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, ftype)) {
        llvm::errs() << "TargetMachine can't emit a file of this type";
        return 1;
    }

    pass.run(*mainModule);
    dest.flush();
    return 0;
}

llvm::Value* CodegenVisitor::codegen() 
{
    return ast->accept(*this);
}

llvm::Value* CodegenVisitor::codegen(ExpressionAST* ast) 
{
    // handle special case of variable declaration / reassignment
    // in this case, we don't want to codegen LHS as a separate expression
    if (ast->op == Operator::OP_EQL)
    {
        // should only be using equals when variable is LHS
        auto varAst = dynamic_cast<VariableAST*>(ast->LHS->clone());
        llvm::Value* rhs = ast->RHS->accept(*this);
        if (varAst->ctx == VarCtx::eAlloc)
        {
            if (namedValues[varAst->id])
                throw ReferenceError(mainModule->getModuleIdentifier(), "cannot redefine variable", SourceLocation(0,0));
            // make all allocations in entry block of function
            llvm::AllocaInst* varAlloca = createEntryBlockAlloca(currFunc, rhs);
            namedValues[varAst->id] = varAlloca;
            // store value at current insert point
            builder->CreateStore(rhs, varAlloca);
        }
        else
        {
            llvm::Value* var = namedValues[varAst->id];
            if (!var)
            {
                std::ostringstream s;
                s << "unknown variable name '" << (varAst->id) << '\'';
                throw ReferenceError(mainModule->getModuleIdentifier(), s.str(), SourceLocation(0,0));
            }
            builder->CreateStore(rhs, var);
        }
        return rhs;
    }
    llvm::Value* L = ast->LHS->accept(*this);
    llvm::Value* R = ast->RHS->accept(*this);
    if (!L || !R)
    {
        return nullptr;
    }

    /*
     * CreateOp Notes:
     * ===============
     *
     * NSW = No Signed Wrap
     * NUS = No Unsigned Wrap
     *
     * NEVER DELETE THIS LINK LOL:
     * https://stackoverflow.com/questions/61207663/how-to-use-llvmirbuilder-create-add-sub-mul-div 
     *
     */

    switch (ast->op) {
        case Operator::OP_ADD:
            return builder->CreateAdd(L, R, "addtmp");
        case Operator::OP_SUB:
            return builder->CreateNSWSub(L, R, "subtmp");
        case Operator::OP_MUL:
            return builder->CreateNSWMul(L, R, "multmp");
        case Operator::OP_BOOL_EQL:
            return builder->CreateICmpEQ(L, R, "eqtmp");
        default:
            throw SyntaxError(mainModule->getModuleIdentifier(), "invalid binary operator", SourceLocation(0,0));
    }
}

llvm::Value* CodegenVisitor::codegen(ModuleAST* ast) 
{
    for (auto& child : ast->children)
    {
        child->accept(*this);
    }
    return nullptr;
}

llvm::Value* CodegenVisitor::codegen(NumberAST* ast) 
{
    return llvm::ConstantInt::get(*ctx, llvm::APInt(32, ast->val));
}

llvm::Value* CodegenVisitor::codegen(VariableAST* ast) 
{
    llvm::AllocaInst* val = namedValues[ast->id];
    if (!val)
    {
        std::ostringstream s;
        s << "unknown variable name '" << ast->id << '\'';
        throw ReferenceError(mainModule->getModuleIdentifier(), s.str(), SourceLocation(0,0));
    }
    return builder->CreateLoad(val->getAllocatedType(), val, ast->id);
}

llvm::Function* CodegenVisitor::codegen(FunctionAST* ast) 
{
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
        throw ReferenceError(mainModule->getModuleIdentifier(), "cannot redefine function", SourceLocation(0,0));
    }
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*ctx, "entry", func);
    builder->SetInsertPoint(BB);

    namedValues.clear();
    for (auto& param : func->args())
    {
        llvm::AllocaInst* argAlloca = createEntryBlockAlloca(func, &param);
        builder->CreateStore(&param, argAlloca);
        namedValues[param.getName().str()] = argAlloca;
    }
    
    currFunc = func;
    llvm::Value* retVal = ast->body->accept(*this);
    if (retVal)
    {
        builder->CreateRet(retVal);
        llvm::verifyFunction(*func);
        return func;
    }

    func->eraseFromParent();
    return nullptr;
}

llvm::Value* CodegenVisitor::codegen(CompoundAST* ast) 
{
    // children.size() - 1 to skip return statement (only temporary)
    for (int i = 0; i < ast->children.size(); i++)
    {
        ast->children[i]->accept(*this);
    }
    if (!ast->retStmt)
        return nullptr;
    return ast->retStmt->accept(*this);
}

llvm::Function* CodegenVisitor::codegen(PrototypeAST* ast) 
{
    std::vector<llvm::Type*> paramTypes;
    for (auto& param : ast->params)
    {
        paramTypes.push_back(typeToLlvm(param->type));
    }
    llvm::FunctionType *funcType = llvm::FunctionType::get(typeToLlvm(ast->retType), paramTypes, false);
    llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, ast->name, mainModule.get());

    int i = 0;
    for (auto& param : func->args())
    {
        param.setName(ast->params[i++]->id);
    }
    return func;
}

llvm::Value* CodegenVisitor::codegen(ReturnAST* ast) 
{
    return ast->expr->accept(*this);
}

llvm::Value* CodegenVisitor::codegen(CallAST* ast) 
{
    llvm::Function* callee = mainModule->getFunction(ast->id);
    if (!callee)
    {
        std::ostringstream s;
        s << "unknown function name '" << ast->id << "'\n";
        throw ReferenceError(mainModule->getModuleIdentifier(), s.str(), SourceLocation(0,0));
    }
    if (callee->arg_size() != ast->params.size())
    {
        std::ostringstream s;
        s << "incorrect number of arguments passed to function. got " << ast->params.size();
        s << " but expected " << callee->arg_size() << ".\n";
        // TODO: make new error for this (Python uses TypeError)
        throw SyntaxError(mainModule->getModuleIdentifier(), s.str(), SourceLocation(0,0));
    }
    std::vector<llvm::Value*> argValues;
    for (int i = 0; i < ast->params.size(); i++)
    {
        argValues.push_back(ast->params[i]->accept(*this));
        if (!argValues.back())
            return nullptr;
    }
    return builder->CreateCall(callee, argValues, "calltmp");
}

llvm::Value* CodegenVisitor::codegen(IfAST* ast)
{
    llvm::Function* func = builder->GetInsertBlock()->getParent();
    
    // create initial blocks
    llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(*ctx, "if");
    llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(*ctx, "else");
    llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(*ctx, "merge");

    // create branch instruction based on condition
    llvm::Value* cond = ast->expr->accept(*this);
    builder->CreateCondBr(cond, ifBlock, elseBlock);

    // insert if block
    insertFuncBlock(func, ifBlock);
    llvm::Value* thenV = ast->body->accept(*this);
    createRetOrBr(thenV, mergeBlock);
    ifBlock = builder->GetInsertBlock();

    // insert else block
    insertFuncBlock(func, elseBlock);
    if (!ast->other)
    {
        // conditional w/o explicit else block
        // insert block with just one instruction which is a br
        builder->CreateBr(mergeBlock);
        insertFuncBlock(func, mergeBlock);
        return nullptr;
    }
    
    llvm::Value* elseV = ast->other->body->accept(*this);
    createRetOrBr(elseV, mergeBlock);
    elseBlock = builder->GetInsertBlock();
    insertFuncBlock(func, mergeBlock);

    // TODO: implement PHI node functionality. Currently just inserting ret or
    // br without PHI

    // if (!thenV || !elseV)
    //     return nullptr;

    // builder->SetInsertPoint(ifBlock);
    // builder->SetInsertPoint(elseBlock);

    // // if both have a return statement, use a PHI node
    // llvm::PHINode* pn = builder->CreatePHI(typeToLlvm(Type::eInt), 2, "iftmp");
    // pn->addIncoming(thenV, ifBlock);
    // pn->addIncoming(elseV, ifBlock);
    // return pn;

    return nullptr;
}

void CodegenVisitor::debugPrint(IfAST* ast)
{
    std::cout << "br ";
    if (ast->other)
    {
        // if statement with another branch after it
        if (ast->other->other)
        {
            // if statement with an else if after it
            std::cout << "true block, cond block\n";
            debugPrint(ast->other.get());
        }
        else
        {
            std::cout << "true block, false block\n";
            // if statement with an else after it
        }
    }
    else
    {
        // if statement without else
        std::cout << "true block, br block\n";
    }
    std::cout << "merge\n";
}

void CodegenVisitor::insertFuncBlock(llvm::Function* func, llvm::BasicBlock* block)
{
    // Used in control flow
    // Appends a block to the function's list of blocks and sets the
    // IRBuilder's insert point to the newly appended block
    func->getBasicBlockList().push_back(block);
    builder->SetInsertPoint(block);
}

void CodegenVisitor::createRetOrBr(llvm::Value* retV, llvm::BasicBlock* block)
{
    // Used in control flow (primarily if-else if-else-statements)
    // Inserts a branch ('br') or return ('ret') instruction based on if the
    // return value is not null
    if (retV)
        builder->CreateRet(retV);
    else
        builder->CreateBr(block);
}

llvm::Type* CodegenVisitor::typeToLlvm(Type type)
{
    switch (type)
    {
        case Type::eInt:
            return llvm::Type::getInt32Ty(*ctx);
        case Type::eVoid:
            return llvm::Type::getVoidTy(*ctx);
        default:
            std::cout << "ERROR - UNHANDLED CODEGEN TYPE. STOP.\n";
            exit(1);
    }
}

llvm::AllocaInst* CodegenVisitor::createEntryBlockAlloca(llvm::Function* func, llvm::Value* val)
{
    llvm::BasicBlock& bb = func->getEntryBlock();
    llvm::IRBuilder<> tmpBuilder(&bb, bb.begin());
    return tmpBuilder.CreateAlloca(val->getType(), 0, val->getName());
}
