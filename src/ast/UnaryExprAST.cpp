#include "UnaryExprAST.h"

UnaryExprAST::UnaryExprAST(std::unique_ptr<AST> operand, Operator::op_type op)
    : operand(std::move(operand)), op(op) {}

UnaryExprAST::UnaryExprAST(const UnaryExprAST& other)
    : operand(std::unique_ptr<AST>(other.operand->clone())), op(other.op) {}

llvm::Value* UnaryExprAST::accept(ASTCodegenner& cg) 
{
    return cg.codegen(this);
}

Type UnaryExprAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

std::string UnaryExprAST::accept(ASTStringifier& sf, int tabs) 
{
    return sf.toString(this, tabs);
}

UnaryExprAST* UnaryExprAST::cloneImpl()
{
    return new UnaryExprAST(*this);
}
