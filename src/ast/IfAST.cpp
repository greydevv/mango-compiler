#include "IfAST.h"
#include "CompoundAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

IfAST::IfAST(std::unique_ptr<AST> expr, std::unique_ptr<CompoundAST> body, std::unique_ptr<IfAST> other)
    : expr(std::move(expr)),
      body(std::move(body)),
      other(std::move(other)) {}

IfAST::IfAST(const IfAST& other)
    : expr(std::unique_ptr<AST>(other.expr->clone())),
      body(std::unique_ptr<CompoundAST>(dynamic_cast<CompoundAST*>(other.body->clone()))),
      other(std::unique_ptr<IfAST>(dynamic_cast<IfAST*>(other.other->clone()))) {}

llvm::Value* IfAST::accept(CodegenVisitor& cg) 
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
