#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include <vector>
#include <memory>
#include "AST.h"
#include "VariableAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

class PrototypeAST : public AST
{
    public:
        std::vector<std::unique_ptr<VariableAST>> params;


    protected:
        virtual PrototypeAST* cloneImpl() override;
};

#endif
