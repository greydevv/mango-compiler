#include "AST.h"
#include "ExpressionAST.h"
#include "../Operator.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

ExpressionAST::ExpressionAST(std::unique_ptr<AST> LHS, std::unique_ptr<AST> RHS, Operator::op_type op) 
    : LHS(std::move(LHS)), RHS(std::move(RHS)), op(op) {}

ExpressionAST::ExpressionAST(const ExpressionAST& other)
    : LHS(std::unique_ptr<AST>(other.LHS->clone())), 
      RHS(std::unique_ptr<AST>(other.RHS->clone())), 
      op(other.op) {}

llvm::Value* ExpressionAST::accept(ASTCodegenner& cg) 
{
    return cg.codegen(this);
}

Type ExpressionAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

std::string ExpressionAST::accept(ASTStringifier& sf, int tabs) 
{
    return sf.toString(this, tabs);
}

ExpressionAST* ExpressionAST::cloneImpl()
{
    return new ExpressionAST(*this);
}
