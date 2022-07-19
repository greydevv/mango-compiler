#ifndef EXPRESSION_AST_H
#define EXPRESSION_AST_H

#include <string>
#include <memory>
#include "AST.h"
#include "../Operator.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

enum class ExprCtx
{
    eStore,
    eReference,
    eReturn
};

class ExpressionAST : public AST
{
    public:
        std::unique_ptr<AST> LHS;
        std::unique_ptr<AST> RHS;
        Operator::op_type op;

        ExpressionAST(std::unique_ptr<AST> LHS, std::unique_ptr<AST> RHS, Operator::op_type op);
        ExpressionAST(const ExpressionAST& other);
        virtual Type accept(ASTValidator& vd) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual ExpressionAST* cloneImpl() override;
};

#endif
