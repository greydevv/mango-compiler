#ifndef CODEGEN_H
#define CODEGEN_H

#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"

class ExpressionAST;
class ModuleAST;
class VariableAST;
class NumberAST;
class FunctionAST;
class CompoundAST;
class PrototypeAST;
class ReturnAST;

class CodegenVisitor
{
    public:
        CodegenVisitor(const std::string& fname, std::unique_ptr<ModuleAST> ast);
        llvm::Value* codegen(ExpressionAST* ast);
        llvm::Value* codegen(ModuleAST* ast);
        llvm::Value* codegen(VariableAST* ast);
        llvm::Value* codegen(NumberAST* ast);
        llvm::Value* codegen(FunctionAST* ast);
        llvm::Value* codegen(CompoundAST* ast);
        llvm::Value* codegen(PrototypeAST* ast);
        llvm::Value* codegen(ReturnAST* ast);

    private:
        std::unique_ptr<ModuleAST> ast;
        std::unique_ptr<llvm::LLVMContext> ctx;
        std::unique_ptr<llvm::IRBuilder<>> builder;
        std::unique_ptr<llvm::Module> mainModule;
        std::map<std::string, llvm::AllocaInst*> namedValues;
};

#endif
