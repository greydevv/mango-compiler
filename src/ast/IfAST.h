#ifndef IF_AST_H
#define IF_AST_H

#include "AST.h"
#include "CompoundAST.h"
#include "../visitors/CodegenVisitor.h"
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
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual IfAST* cloneImpl() override;
};

#endif
