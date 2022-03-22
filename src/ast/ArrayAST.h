#ifndef ARRAY_AST_H
#define ARRAY_AST_H

#include "AST.h"
#include "../Types.h"

class ArrayAST : public AST
{
    public:
        Type type;
        std::vector<std::unique_ptr<AST>> elements;

        ArrayAST(Type type, std::vector<std::unique_ptr<AST>> elements);
        ArrayAST(Type type);
        ArrayAST(const ArrayAST& other);
        void addElement(std::unique_ptr<AST> ele);
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual ArrayAST* cloneImpl() override;
};

#endif
