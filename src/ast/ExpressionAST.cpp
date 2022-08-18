#include <string>
#include <utility>
#include "AST.h"
#include "ExpressionAST.h"
#include "../Operator.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

ExpressionAST::ExpressionAST(std::unique_ptr<AST> LHS, std::unique_ptr<AST> RHS, Operator::op_type op, SourceLocation opLoc)
    : AST(LHS->loc), op(op), opLoc(opLoc), LHS(std::move(LHS)), RHS(std::move(RHS))
{
    calculateLoc();
}

ExpressionAST::ExpressionAST(std::unique_ptr<AST> LHS)
    : AST(LHS->loc), op(Operator::OP_NOP), opLoc(SourceLocation(-1, -1, -1)), LHS(std::move(LHS)), RHS(nullptr)
{
    calculateLoc();
}

std::shared_ptr<AST> ExpressionAST::getLhs()
{
    return LHS;
}

std::shared_ptr<AST> ExpressionAST::getRhs()
{
    return RHS;
}

void ExpressionAST::setRhs(std::unique_ptr<AST> rhs)
{
    this->RHS = std::move(rhs);
    calculateLoc();
}

ExpressionAST::ExpressionAST(const ExpressionAST& other)
    : AST(other.loc),
      op(other.op),
      opLoc(other.opLoc),
      LHS(std::unique_ptr<AST>(other.LHS->clone())),
      RHS(other.RHS ? std::unique_ptr<AST>(other.RHS->clone()) : nullptr) {}

bool ExpressionAST::isAssignable()
{
    // can assign if no operation is taking place (i.e. no operator and no right-hand side)
    return op == Operator::OP_NOP && !RHS && LHS->isAssignable();
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
    if (RHS)
      loc.len = (RHS->loc.x + RHS->loc.len) - loc.x;
}
