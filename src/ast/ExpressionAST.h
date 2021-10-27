#ifndef EXPRESSION_AST_H
#define EXPRESSION_AST_H

#include <memory>
#include "AST.h"
#include "../Operator.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

class ExpressionAST : public AST
{
    public:
        std::unique_ptr<AST> LHS;
        std::unique_ptr<AST> RHS;
        Operator::op_type op;

        ExpressionAST(std::unique_ptr<AST> LHS, std::unique_ptr<AST> RHS, Operator::op_type op);
        ExpressionAST(const ExpressionAST& other);
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf) override;
    protected:
        virtual ExpressionAST* cloneImpl() override;
};

#endif