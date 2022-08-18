#ifndef WHILE_AST_H
#define WHILE_AST_H

#include <string>
#include <memory>
#include "ExpressionAST.h"
#include "CompoundAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class WhileAST : public AST
{
    public:
        std::shared_ptr<ExpressionAST> expr;
        std::unique_ptr<CompoundAST> body;

        WhileAST(std::shared_ptr<ExpressionAST> expr, std::unique_ptr<CompoundAST> body);
        WhileAST(const WhileAST& other);
        Type accept(ASTValidator& cg) override;
        llvm::Value* accept(ASTCodegenner& cg) override;
        std::string accept(ASTStringifier& sf, int tabs = 0) override;

    protected:
        WhileAST* cloneImpl() override;
};

#endif
