#include <utility>
#include "ExpressionAST.h"
#include "WhileAST.h"
#include "CompoundAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

WhileAST::WhileAST(std::shared_ptr<ExpressionAST> expr, std::unique_ptr<CompoundAST> body)
    : expr(expr), body(std::move(body)) {}

WhileAST::WhileAST(const WhileAST& other)
    : expr(std::shared_ptr<ExpressionAST>(dynamic_cast<ExpressionAST*>(other.expr->clone()))),
      body(std::unique_ptr<CompoundAST>(dynamic_cast<CompoundAST*>(other.expr->clone()))) {}

Type WhileAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

llvm::Value* WhileAST::accept(ASTCodegenner& cg)
{
    return cg.codegen(this);
}

std::string WhileAST::accept(ASTStringifier& sf, int tabs)
{
    return sf.toString(this, tabs);
}

WhileAST* WhileAST::cloneImpl()
{
    return new WhileAST(*this);
}
