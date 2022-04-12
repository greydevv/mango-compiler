#include "AST.h"
#include "ReturnAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

ReturnAST::ReturnAST(std::unique_ptr<AST> expr) 
    : expr(std::move(expr)) {}

ReturnAST::ReturnAST(const ReturnAST& other)
    : expr(std::unique_ptr<AST>(other.expr->clone())) {}

std::unique_ptr<ReturnAST> ReturnAST::retVoid()
{
    return std::make_unique<ReturnAST>(nullptr);
}

bool ReturnAST::hasExpr()
{
    return (expr != nullptr);
}

Type ReturnAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

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
