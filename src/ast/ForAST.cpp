#include <memory>
#include "ForAST.h"
#include "CompoundAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

ForAST::ForAST(std::unique_ptr<ExpressionAST> var, std::unique_ptr<ExpressionAST> expr, std::unique_ptr<ExpressionAST> stepExpr, std::unique_ptr<CompoundAST> body)
    : var(std::move(var)), expr(std::move(expr)), stepExpr(std::move(stepExpr)), body(std::move(body)) {}

ForAST::ForAST(const ForAST& other)
    : var(std::unique_ptr<ExpressionAST>(dynamic_cast<ExpressionAST*>(other.var->clone()))),
      expr(std::unique_ptr<ExpressionAST>(dynamic_cast<ExpressionAST*>(other.expr->clone()))),
      stepExpr(std::unique_ptr<ExpressionAST>(dynamic_cast<ExpressionAST*>(other.stepExpr->clone()))), 
      body(std::unique_ptr<CompoundAST>(dynamic_cast<CompoundAST*>(other.body->clone()))) {}

llvm::Value* ForAST::accept(CodegenVisitor& cg) 
{
    return cg.codegen(this);
}

std::string ForAST::accept(ASTStringifier& sf, int tabs) 
{
    return sf.toString(this, tabs);
}

ForAST* ForAST::cloneImpl()
{
    return new ForAST(*this);
}
