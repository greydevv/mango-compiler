#ifndef UNARY_EXPR_AST
#define UNARY_EXPR_AST

#include <memory>
#include "AST.h"
#include "../Operator.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class UnaryExprAST : public AST
{
    public:
        std::unique_ptr<AST> operand;
        Operator::op_type op;

        UnaryExprAST(std::unique_ptr<AST> operand, Operator::op_type op);
        UnaryExprAST(const UnaryExprAST& other);
        virtual Type accept(ASTValidator& vd) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual UnaryExprAST* cloneImpl() override;
};

#endif
