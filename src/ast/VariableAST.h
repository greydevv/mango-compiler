#ifndef VARIABLE_AST_H
#define VARIABLE_AST_H

#include <string>
#include "AST.h"
#include "../Types.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

enum class VarCtx
{
    eReference, // referencing a variable (3 * a * 20)
    eStore, // storing new value in a variable (a = ...)
    eAlloc, // allocating a vairable (int a = ...) 
    eParam, // function parameter (func add(int a, intb))
};

class VariableAST : public AST 
{
    public:
        std::string id;
        Type type;
        VarCtx ctx;

        VariableAST(const std::string& id, Type type, VarCtx ctx);
        VariableAST(const std::string& id);
        VariableAST(const VariableAST& other);
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual VariableAST* cloneImpl() override;
};

#endif
