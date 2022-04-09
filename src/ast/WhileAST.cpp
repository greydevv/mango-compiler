#include "AST.h"
#include "WhileAST.h"
#include "CompoundAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

WhileAST::WhileAST(std::unique_ptr<AST> expr, std::unique_ptr<CompoundAST> body)
    : expr(std::move(expr)), body(std::move(body)) {}

WhileAST::WhileAST(const WhileAST& other)
    : expr(std::unique_ptr<AST>(other.expr->clone())),
      body(std::unique_ptr<CompoundAST>(dynamic_cast<CompoundAST*>(other.expr->clone()))) {}

bool WhileAST::accept(ASTValidator& vd)
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
