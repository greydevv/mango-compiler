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
        Type accept(ASTValidator& vd) override;
        llvm::Value* accept(ASTCodegenner& cg) override;
        std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        NumberAST* cloneImpl() override;
};

#endif
