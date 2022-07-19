#ifndef ARRAY_AST_H
#define ARRAY_AST_H

#include <string>
#include <memory>
#include <vector>
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
        Type accept(ASTValidator& vd) override;
        llvm::Value* accept(ASTCodegenner& cg) override;
        std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        ArrayAST* cloneImpl() override;
};

#endif
