#include <utility>
#include "AST.h"
#include "UnaryExprAST.h"
#include "../Operator.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

UnaryExprAST::UnaryExprAST(std::unique_ptr<AST> operand, Operator::op_type op, unary_type type)
    : operand(std::move(operand)), op(op), type(type) {}

UnaryExprAST::UnaryExprAST(const UnaryExprAST& other)
    : operand(std::unique_ptr<AST>(other.operand->clone())), op(other.op) {}

bool UnaryExprAST::isPrefix()
{
    return type == UNARY_PRE;
}

bool UnaryExprAST::isPostfix()
{
    return type == UNARY_POST;
}

bool UnaryExprAST::isInc()
{
    return op == Operator::OP_INC;
}

bool UnaryExprAST::isDec()
{
    return op == Operator::OP_DEC;
}

std::unique_ptr<UnaryExprAST> UnaryExprAST::unaryPrefix(std::unique_ptr<AST> operand, Operator::op_type op)
{
    return std::make_unique<UnaryExprAST>(std::move(operand), op, UNARY_PRE);
}

std::unique_ptr<UnaryExprAST> UnaryExprAST::unaryPostfix(std::unique_ptr<AST> operand, Operator::op_type op)
{
    return std::make_unique<UnaryExprAST>(std::move(operand), op, UNARY_POST);
}

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
