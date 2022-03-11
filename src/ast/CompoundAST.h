#ifndef COMPOUND_AST_H
#define COMPOUND_AST_H

#include <memory>
#include <vector>
#include "AST.h"
#include "ReturnAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

class CompoundAST : public AST
{
    public:
        std::vector<std::unique_ptr<AST>> children;
        std::unique_ptr<ReturnAST> retStmt;

        CompoundAST(std::vector<std::unique_ptr<AST>> children, std::unique_ptr<ReturnAST> retStmt);
        CompoundAST(std::vector<std::unique_ptr<AST>> children);
        CompoundAST(const CompoundAST& other);
        void addChild(std::unique_ptr<AST> child);
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual CompoundAST* cloneImpl() override;
};

#endif
