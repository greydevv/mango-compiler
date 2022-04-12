#include <iostream>
#include <string>
#include "ASTValidator.h"
#include "../io.h"
#include "../Types.h"
#include "../ast/AST.h"
#include "../ast/ModuleAST.h"
#include "../ast/ExpressionAST.h"
#include "../ast/VariableAST.h"
#include "../ast/NumberAST.h"
#include "../ast/ArrayAST.h"
#include "../ast/CompoundAST.h"
#include "../ast/FunctionAST.h"
#include "../ast/PrototypeAST.h"
#include "../ast/ReturnAST.h"
#include "../ast/CallAST.h"
#include "../ast/IfAST.h"
#include "../ast/ForAST.h"
#include "../ast/WhileAST.h"
#include "../Exception.h"

ASTValidator::ASTValidator(const std::string& fname, std::shared_ptr<ModuleAST> ast)
    : fname(fname), ast(ast), st() {}

void ASTValidator::validate()
{
    ast->accept(*this);
}

Type ASTValidator::validate(ModuleAST* ast)
{
    for (auto& child : ast->children)
    {
        child->accept(*this);
    }
    return Type::eUnd;
}

Type ASTValidator::validate(ExpressionAST* ast)
{
    Type lType = ast->LHS->accept(*this);
    Type rType = ast->RHS->accept(*this);

    if (lType != rType)
    {
        if (ast->op == Operator::OP_EQL)
            throw TypeError(fname, fmt::format("Cannot initialize {} with a value of {}", typeValues[lType], typeValues[rType]), "N/A", SourceLocation(0,0));
        else
            throw TypeError(fname, fmt::format("{} and {} are not compatible in binary expression", typeValues[lType], typeValues[rType]), "N/A", SourceLocation(0,0));
    }
    return lType;
}

Type ASTValidator::validate(VariableAST* ast)
{
    switch (ast->ctx)
    {
        case VarCtx::eAlloc:
        {
            if (st.contains(ast->id))
            {
                throw ReferenceError(fname, fmt::format("Variable '{}' was already defined", ast->id), "N/A", SourceLocation(0,0));
            }
            else
            {
                st.insert(ast->id, ast->type);
                return ast->type;
            }
        }
        case VarCtx::eParam:
        {
            st.insert(ast->id, ast->type);
            return ast->type;
        }
        case VarCtx::eReference:
        case VarCtx::eStore:
        {
            if (!st.contains(ast->id))
                throw ReferenceError(fname, fmt::format("Reference to unknown variable '{}'", ast->id), "N/A", SourceLocation(0,0));
            // give AST node the type as it is unknown during syntax analysis
            // (parser) but known during semantic analysis (here)
            Type varType = st.lookup(ast->id);
            ast->type = varType;
            return varType;
        }
    }
}

Type ASTValidator::validate(NumberAST* ast)
{
    return Type::eInt;
}

Type ASTValidator::validate(ArrayAST* ast)
{
    throw NotImplementedError("Validation of ArrayAST");
}

Type ASTValidator::validate(CompoundAST* ast)
{
    for (auto& child : ast->children)
        child->accept(*this);
    return ast->retStmt->accept(*this);
}

Type ASTValidator::validate(FunctionAST* ast)
{
    Type expectedType = ast->proto->accept(*this);
    ast->body->accept(*this);
    if (ast->body->retStmt)
    {
        // actual return type
        Type actualType = ast->body->retStmt->accept(*this);
        if (actualType != expectedType)
            throw TypeError(fname, fmt::format("returning '{}' but expected '{}'", typeValues[actualType], typeValues[expectedType]), "N/A", SourceLocation(0,0));
    }
    return expectedType;
}

Type ASTValidator::validate(PrototypeAST* ast)
{
    if (st.contains(ast->name))
        throw ReferenceError(fname, fmt::format("Function '{}' was already defined", ast->name), "N/A", SourceLocation(0,0));
    st.insert(ast->name, ast->retType);
    for (auto& param : ast->params)
        param->accept(*this);
    return ast->retType;
}

Type ASTValidator::validate(ReturnAST* ast)
{
    if (ast->expr)
        return ast->expr->accept(*this);
    return Type::eVoid;
}

Type ASTValidator::validate(CallAST* ast)
{
    if (!st.contains(ast->id))
        throw ReferenceError(fname, fmt::format("Reference to unknown function '{}'", ast->id), "N/A", SourceLocation(0,0));
    return st.lookup(ast->id);
}

Type ASTValidator::validate(IfAST* ast)
{
    throw NotImplementedError("Validation of IfAST");
}

Type ASTValidator::validate(ForAST* ast)
{
    throw NotImplementedError("Validation of ForAST");
}

Type ASTValidator::validate(WhileAST* ast)
{
    throw NotImplementedError("Validation of WhileAST");
}
