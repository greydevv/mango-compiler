#ifndef CODEGEN_H
#define CODEGEN_H

#include <string>
#include "../Types.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"

class ExpressionAST;
class ModuleAST;
class VariableAST;
class NumberAST;
class FunctionAST;
class CompoundAST;
class PrototypeAST;
class ReturnAST;
class CallAST;

class CodegenVisitor
{
    public:
        CodegenVisitor(const std::string& fname, std::unique_ptr<ModuleAST> ast);
        void print();
        int emitObjectCode();
        llvm::Value* codegen();
        llvm::Value* codegen(ExpressionAST* ast);
        llvm::Value* codegen(ModuleAST* ast);
        llvm::Value* codegen(VariableAST* ast);
        llvm::Value* codegen(NumberAST* ast);
        llvm::Function* codegen(FunctionAST* ast);
        llvm::Value* codegen(CompoundAST* ast);
        llvm::Function* codegen(PrototypeAST* ast);
        llvm::Value* codegen(ReturnAST* ast);
        llvm::Value* codegen(CallAST* ast);

    private:
        llvm::Type* typeToLlvm(Type type);
        std::unique_ptr<ModuleAST> ast;
        std::unique_ptr<llvm::LLVMContext> ctx;
        std::unique_ptr<llvm::IRBuilder<>> builder;
        std::unique_ptr<llvm::Module> mainModule;
        llvm::Function* currFunc;
        std::map<std::string, llvm::AllocaInst*> namedValues;
        llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* func, llvm::Value* param);
};

#endif
