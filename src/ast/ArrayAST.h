#ifndef ARRAY_AST_H
#define ARRAY_AST_H

#include "AST.h"
#include "../Types.h"
#include "../visitors/ASTValidator.h"

class ArrayAST : public AST
{
    public:
        Type type;
        std::vector<std::unique_ptr<AST>> elements;

        ArrayAST(Type type, std::vector<std::unique_ptr<AST>> elements);
        ArrayAST(Type type);
        ArrayAST(const ArrayAST& other);
        void addElement(std::unique_ptr<AST> ele);
        virtual bool accept(ASTValidator& vd) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual ArrayAST* cloneImpl() override;
};

#endif
