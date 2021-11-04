#ifndef VARIABLE_AST_H
#define VARIABLE_AST_H

#include <string>
#include "AST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

class VariableAST : public AST 
{
    public:
        std::string id;

        VariableAST(const std::string& id);
        VariableAST(const VariableAST& other);
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual VariableAST* cloneImpl() override;
};

#endif
