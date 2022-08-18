#include <string>
#include <utility>
#include "AST.h"
#include "ExpressionAST.h"
#include "../Operator.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

ExpressionAST::ExpressionAST(std::unique_ptr<AST> lhs, std::unique_ptr<AST> rhs, Operator::op_type op, SourceLocation opLoc)
    : AST(lhs->loc), op(op), opLoc(opLoc), lhs(std::move(lhs)), rhs(std::move(rhs))
{
    calculateLoc();
}

ExpressionAST::ExpressionAST(std::unique_ptr<AST> lhs)
    : AST(lhs->loc), op(Operator::OP_NOP), lhs(std::move(lhs)), rhs(nullptr)
{
    calculateLoc();
}

std::shared_ptr<AST> ExpressionAST::getLhs()
{
    return lhs;
}

std::shared_ptr<AST> ExpressionAST::getRhs()
{
    return rhs;
}

void ExpressionAST::setRhs(std::unique_ptr<AST> rhs)
{
    this->rhs = std::move(rhs);
    calculateLoc();
}

ExpressionAST::ExpressionAST(const ExpressionAST& other)
    : AST(other.loc),
      op(other.op),
      opLoc(other.opLoc),
      lhs(std::unique_ptr<AST>(other.lhs->clone())),
      rhs(other.rhs ? std::unique_ptr<AST>(other.rhs->clone()) : nullptr) {}

bool ExpressionAST::isAssignable()
{
    // can assign if no operation is taking place (i.e. no operator and no right-hand side)
    return op == Operator::OP_NOP && !rhs && lhs->isAssignable();
}

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

void ExpressionAST::calculateLoc()
{
    if (rhs)
      loc.len = (rhs->loc.x + rhs->loc.len) - loc.x;
}
