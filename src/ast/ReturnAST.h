#ifndef RETURN_AST_H
#define RETURN_AST_H

#include <string>
#include <memory>
#include "ExpressionAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

class ReturnAST : public AST
{
    public:
        std::unique_ptr<ExpressionAST> expr;

        ReturnAST(std::unique_ptr<ExpressionAST> expr);
        ReturnAST(const ReturnAST& other);
        static std::unique_ptr<ReturnAST> retVoid();
        bool hasExpr();
        Type accept(ASTValidator& vd) override;
        llvm::Value* accept(ASTCodegenner& cg) override;
        std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        ReturnAST* cloneImpl() override;
};

#endif
