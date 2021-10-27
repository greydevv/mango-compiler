#include "CodegenVisitor.h"
#include "../ast/ExpressionAST.h"
#include "../ast/ModuleAST.h"
#include "../ast/NumberAST.h"
#include "../ast/VariableAST.h"
#include "llvm/IR/Value.h"

llvm::Value* CodegenVisitor::codegen(ExpressionAST* ast) {
    return nullptr;    
}

llvm::Value* CodegenVisitor::codegen(ModuleAST* ast) {
    return nullptr;
}

llvm::Value* CodegenVisitor::codegen(NumberAST* ast) {
    return nullptr;
}

llvm::Value* CodegenVisitor::codegen(VariableAST* ast) {
    return nullptr;
}