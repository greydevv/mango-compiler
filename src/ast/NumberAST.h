#ifndef NUMBER_AST_H
#define NUMBER_AST_H

#include <string>
#include "AST.h"
#include "../Types.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class NumberAST : public AST
{
    public:
        double val;
        Type type;

        NumberAST(double val, Type type);
        NumberAST(double val);
        NumberAST(const NumberAST& other);
        virtual Type accept(ASTValidator& vd) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual NumberAST* cloneImpl() override;
};

#endif
