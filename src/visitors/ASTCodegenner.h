#ifndef CODEGEN_H
#define CODEGEN_H

#include <string>
#include <memory>
#include <map>
#include "../ContextManager.h"
#include "../Types.h"
#include "../Operator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"

class ExpressionAST;
class UnaryExprAST;
class ModuleAST;
class VariableAST;
class NumberAST;
class ArrayAST;
class FunctionAST;
class CompoundAST;
class PrototypeAST;
class ReturnAST;
class CallAST;
class IfAST;
class ForAST;
class WhileAST;

class ASTCodegenner
{
    public:
        ASTCodegenner(std::shared_ptr<ModuleAST> ast, ContextManager& ctx);
        std::string print();
        int emitObjectCode();
        llvm::Value* codegen();
        llvm::Value* codegen(ExpressionAST* ast);
        llvm::Value* codegen(UnaryExprAST* ast);
        llvm::Value* codegen(ModuleAST* ast);
        llvm::Value* codegen(VariableAST* ast);
        llvm::Value* codegen(NumberAST* ast);
        llvm::Value* codegen(ArrayAST* ast);
        llvm::Function* codegen(FunctionAST* ast);
        llvm::Value* codegen(CompoundAST* ast);
        llvm::Function* codegen(PrototypeAST* ast);
        llvm::Value* codegen(ReturnAST* ast);
        llvm::Value* codegen(CallAST* ast);
        llvm::Value* codegen(IfAST* ast);
        llvm::Value* codegen(IfAST* ast, llvm::BasicBlock* parentMergeBlock);
        llvm::Value* codegen(ForAST* ast);
        llvm::Value* codegen(WhileAST* ast);

    private:
        void debugPrint(IfAST* ast);
        void insertFuncBlock(llvm::Function* func, llvm::BasicBlock* block);
        void createRetOrBr(std::shared_ptr<CompoundAST> body, llvm::BasicBlock* block);
        llvm::Value* applyUnaryOperation(llvm::Value* res, Operator::op_type op);
        // void createRetOrBr(llvm::Value* retV, llvm::BasicBlock* block);
        llvm::Type* typeToLlvm(Type type);
        std::shared_ptr<ModuleAST> ast;
        ContextManager& ctx;
        std::unique_ptr<llvm::LLVMContext> llvmCtx;
        std::unique_ptr<llvm::IRBuilder<>> builder;
        std::unique_ptr<llvm::Module> mainModule;
        std::map<std::string, llvm::AllocaInst*> namedValues;
        llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* func, llvm::Value* val);
        llvm::AllocaInst* createEntryBlockAlloca(llvm::Value* val);
};

#endif
