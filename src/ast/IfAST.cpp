#include <utility>
#include "IfAST.h"
#include "CompoundAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

IfAST::IfAST(std::shared_ptr<ExpressionAST> expr, std::unique_ptr<CompoundAST> body, std::unique_ptr<IfAST> other)
    : expr(std::move(expr)),
      body(std::move(body)),
      other(std::move(other)) {}

IfAST::IfAST(const IfAST& other)
    : expr(other.expr ? std::shared_ptr<ExpressionAST>(dynamic_cast<ExpressionAST*>(other.expr->clone())) : nullptr),
      body(other.body ? std::shared_ptr<CompoundAST>(dynamic_cast<CompoundAST*>(other.body->clone())) : nullptr),
      other(other.other ? std::shared_ptr<IfAST>(dynamic_cast<IfAST*>(other.other->clone())) : nullptr) {}

Type IfAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

llvm::Value* IfAST::accept(ASTCodegenner& cg)
{
    return cg.codegen(this);
}

std::string IfAST::accept(ASTStringifier& sf, int tabs)
{
    return sf.toString(this, tabs);
}

IfAST* IfAST::cloneImpl()
{
    return new IfAST(*this);
}
