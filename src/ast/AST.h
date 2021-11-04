#ifndef AST_H
#define AST_H

#include "llvm/IR/Value.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

class AST
{
    public:
        virtual ~AST() {};
        virtual llvm::Value* accept(CodegenVisitor& cg) = 0;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) = 0;
        virtual AST* clone();
    protected:
        virtual AST* cloneImpl() = 0;
};

#endif