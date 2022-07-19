#ifndef VARIABLE_AST_H
#define VARIABLE_AST_H

#include <string>
#include "AST.h"
#include "../Token.h"
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
        SourceLocation loc;

        VariableAST(const std::string& id, Type type, VarCtx ctx, SourceLocation loc);
        VariableAST(const std::string& id, VarCtx ctx, SourceLocation loc);
        VariableAST(const std::string& id, SourceLocation loc);
        VariableAST(const VariableAST& other);
        virtual bool isAssignable() override;
        virtual Type accept(ASTValidator& vd) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual VariableAST* cloneImpl() override;
};

#endif
