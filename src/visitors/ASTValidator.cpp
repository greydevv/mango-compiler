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
    // throw NotImplementedError("Validation of ExpressionAST");
    ast->LHS->accept(*this);
    ast->RHS->accept(*this);

    return Type::eUnd;
}

Type ASTValidator::validate(VariableAST* ast)
{
    switch (ast->ctx)
    {
        case VarCtx::eAlloc:
        {
            if (st.contains(ast->id))
                throw ReferenceError(fname, fmt::format("Variable '{}' was already defined", ast->id), "N/A", SourceLocation(0,0));
            else
            {
                st.insert(ast->id, ast->type);
                return ast->type;
            }
        }
        case VarCtx::eParam:
            return ast->type;
        case VarCtx::eReference:
        case VarCtx::eStore:
        {
            if (!st.contains(ast->id))
                throw ReferenceError(fname, fmt::format("Reference to unknown variable '{}'", ast->id), "N/A", SourceLocation(0,0));
            return ast->type;
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
    for (int i = 0; i < ast->children.size(); i++)
    {
        ast->children[i]->accept(*this);
    }
    if (ast->retStmt)
        ast->retStmt->accept(*this);
    return Type::eUnd;
}

Type ASTValidator::validate(FunctionAST* ast)
{
    Type expectedType = ast->proto->retType;
    if (ast->body->retStmt)
    {
        // actual return type
        Type actualType = ast->body->retStmt->accept(*this);
        if (actualType != expectedType)
            throw TypeError(fname, fmt::format("returning '{}' but expected '{}'", typeValues[actualType], typeValues[expectedType]), "N/A", SourceLocation(0,0));
    }

    ast->body->accept(*this);
    return expectedType;
}

Type ASTValidator::validate(PrototypeAST* ast)
{
    // throw NotImplementedError("Validation of PrototypeAST");
    return Type::eUnd;
}

Type ASTValidator::validate(ReturnAST* ast)
{
    if (ast->expr)
        return ast->expr->accept(*this);
    return Type::eVoid;
}

Type ASTValidator::validate(CallAST* ast)
{
    throw NotImplementedError("Validation of CallAST");
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
