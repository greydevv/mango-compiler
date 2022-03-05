#ifndef VARIABLE_AST_H
#define VARIABLE_AST_H

#include <string>
#include "AST.h"
#include "../Types.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

enum class VarCtx
{
    eReference, // referencing a variable
    eStore, // declaring a variable
};

class VariableAST : public AST 
{
    public:
        std::string id;
        Type type;
        VarCtx ctx;

        VariableAST(const std::string& id, Type type = Type::eUnd, VarCtx ctx = VarCtx::eReference);
        VariableAST(const VariableAST& other);
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual VariableAST* cloneImpl() override;
};

#endif
