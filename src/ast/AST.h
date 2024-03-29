#ifndef AST_H
#define AST_H

#include <string>
#include "../Token.h"
#include "llvm/IR/Value.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class AST
{
    public:
        SourceLocation loc;

        AST() {};
        AST(SourceLocation loc);
        virtual ~AST() {};
        virtual bool isAssignable();
        virtual Type accept(ASTValidator& vd) = 0;
        virtual llvm::Value* accept(ASTCodegenner& cg) = 0;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) = 0;
        virtual AST* clone();
    protected:
        virtual AST* cloneImpl() = 0;
};

#endif
