#ifndef NUMBER_AST_H
#define NUMBER_AST_H

#include "AST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

class NumberAST : public AST 
{
    public:
        double val;

        NumberAST(double val);
        NumberAST(const NumberAST& other);
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf) override;
    protected:
        virtual NumberAST* cloneImpl() override;
};

#endif
