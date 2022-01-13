#ifndef CODEGEN_H
#define CODEGEN_H

#include "llvm/IR/Value.h"

class ExpressionAST;
class ModuleAST;
class VariableAST;
class NumberAST;
class FunctionAST;
class CompoundAST;
class PrototypeAST;

class CodegenVisitor
{
    public:
        CodegenVisitor() {};
        llvm::Value* codegen(ExpressionAST* ast);
        llvm::Value* codegen(ModuleAST* ast);
        llvm::Value* codegen(VariableAST* ast);
        llvm::Value* codegen(NumberAST* ast);
        llvm::Value* codegen(FunctionAST* ast);
        llvm::Value* codegen(CompoundAST* ast);
        llvm::Value* codegen(PrototypeAST* ast);
};

#endif
