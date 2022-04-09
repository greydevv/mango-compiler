#include "AST.h"
#include "ReturnAST.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

ReturnAST::ReturnAST(std::unique_ptr<AST> expr) 
    : expr(std::move(expr)) {}

ReturnAST::ReturnAST(const ReturnAST& other)
    : expr(std::unique_ptr<AST>(other.expr->clone())) {}

llvm::Value* ReturnAST::accept(ASTCodegenner& cg) 
{
    return cg.codegen(this);
}

std::string ReturnAST::accept(ASTStringifier& sf, int tabs) 
{
    return sf.toString(this, tabs);
}

ReturnAST* ReturnAST::cloneImpl()
{
    return new ReturnAST(*this);
}
