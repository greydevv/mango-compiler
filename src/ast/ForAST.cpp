#include <string>
#include <memory>
#include <utility>
#include "ForAST.h"
#include "ExpressionAST.h"
#include "ArrayAST.h"
#include "CompoundAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

ForAST::ForAST(std::unique_ptr<ExpressionAST> expr, std::unique_ptr<ArrayAST> iter, std::unique_ptr<CompoundAST> body)
    : expr(std::move(expr)), iter(std::move(iter)), body(std::move(body)) {}

ForAST::ForAST(const ForAST& other)
    : expr(std::unique_ptr<ExpressionAST>(dynamic_cast<ExpressionAST*>(other.expr->clone()))),
      iter(std::unique_ptr<ArrayAST>(dynamic_cast<ArrayAST*>(other.iter->clone()))),
      body(std::unique_ptr<CompoundAST>(dynamic_cast<CompoundAST*>(other.body->clone()))) {}

Type ForAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

llvm::Value* ForAST::accept(ASTCodegenner& cg)
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
