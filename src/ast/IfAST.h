#ifndef IF_AST_H
#define IF_AST_H

#include "AST.h"
#include "CompoundAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

class IfAST : public AST
{
    public:
        std::unique_ptr<AST> expr;
        std::unique_ptr<CompoundAST> body;
        std::unique_ptr<IfAST> other; // else if, else, etc...

        IfAST(std::unique_ptr<AST> expr, std::unique_ptr<CompoundAST> body, std::unique_ptr<IfAST> other);
        IfAST(const IfAST& other);
        virtual bool accept(ASTValidator& cg) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual IfAST* cloneImpl() override;
};

#endif
