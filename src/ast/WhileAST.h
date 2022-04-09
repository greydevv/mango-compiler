#ifndef WHILE_AST_H
#define WHILE_AST_H

#include "AST.h"
#include "CompoundAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class WhileAST : public AST
{
    public:
        std::unique_ptr<AST> expr;
        std::unique_ptr<CompoundAST> body;

        WhileAST(std::unique_ptr<AST> expr, std::unique_ptr<CompoundAST> body);
        WhileAST(const WhileAST& other);
        virtual bool accept(ASTValidator& cg) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;

    protected:
        virtual WhileAST* cloneImpl() override;
};

#endif
