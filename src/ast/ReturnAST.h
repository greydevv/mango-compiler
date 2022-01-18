#ifndef RETURN_AST_H
#define RETURN_AST_H

#include "AST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

class ReturnAST : public AST
{
    public:
        std::unique_ptr<AST> expr;

        ReturnAST(std::unique_ptr<AST> expr);
        ReturnAST(const ReturnAST& other);
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual ReturnAST* cloneImpl() override;
};

#endif
