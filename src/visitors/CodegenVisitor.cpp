#include "CodegenVisitor.h"
#include "../ast/ExpressionAST.h"
#include "../ast/ModuleAST.h"
#include "../ast/NumberAST.h"
#include "../ast/VariableAST.h"
#include "../ast/FunctionAST.h"
#include "../ast/CompoundAST.h"
#include "../ast/PrototypeAST.h"
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

llvm::Value* CodegenVisitor::codegen(FunctionAST* ast) {
    return nullptr;
}

llvm::Value* CodegenVisitor::codegen(CompoundAST* ast) {
    return nullptr;
}

llvm::Value* CodegenVisitor::codegen(PrototypeAST* ast) {
    return nullptr;
}
