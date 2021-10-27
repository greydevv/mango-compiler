#include "AST.h"
#include "ExpressionAST.h"
#include "../Operator.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

ExpressionAST::ExpressionAST(std::unique_ptr<AST> LHS, std::unique_ptr<AST> RHS, Operator::op_type op) 
    : LHS(std::move(LHS)), RHS(std::move(RHS)), op(op) {}

ExpressionAST::ExpressionAST(const ExpressionAST& other)
    : LHS(std::unique_ptr<AST>(other.LHS->clone())), 
      RHS(std::unique_ptr<AST>(other.RHS->clone())), 
      op(other.op) {}

llvm::Value* ExpressionAST::accept(CodegenVisitor& cg) 
{
    return cg.codegen(this);
}

std::string ExpressionAST::accept(ASTStringifier& sf) 
{
    return sf.toString(this);
}

ExpressionAST* ExpressionAST::cloneImpl()
{
    return new ExpressionAST(*this);
}