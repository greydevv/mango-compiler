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

/*
 * Need an expectedType member on ExpressionAST. The ReturnAST would set the
 * ExpressionAST expected type.
 *
 */
class ExpressionAST : public AST
{
    public:
        Operator::op_type op;
        SourceLocation opLoc;

        ExpressionAST(std::unique_ptr<AST> LHS, std::unique_ptr<AST> RHS, Operator::op_type op, SourceLocation opLoc);
        ExpressionAST(std::unique_ptr<AST> LHS);
        ExpressionAST(const ExpressionAST& other);
        std::shared_ptr<AST> getLhs();
        std::shared_ptr<AST> getRhs();
        void setRhs(std::unique_ptr<AST> rhs);
        void setLhs(std::unique_ptr<AST> lhs);
        bool isAssignable() override;
        Type accept(ASTValidator& vd) override;
        llvm::Value* accept(ASTCodegenner& cg) override;
        std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        ExpressionAST* cloneImpl() override;
    private:
        std::shared_ptr<AST> lhs;
        std::shared_ptr<AST> rhs;

        void calculateLoc();
};

#endif
