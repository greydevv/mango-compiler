#include <string>
#include "ASTValidator.h"
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

ASTValidator::ASTValidator(std::shared_ptr<ModuleAST> ast)
    : ast(ast) {}

bool ASTValidator::validate()
{
    return ast->accept(*this);
}

bool ASTValidator::validate(ModuleAST* ast)
{
    throw NotImplementedError("Validation of ModuleAST");
}

bool ASTValidator::validate(ExpressionAST* ast)
{
    throw NotImplementedError("Validation of ExpressionAST");
}

bool ASTValidator::validate(VariableAST* ast)
{
    throw NotImplementedError("Validation of VariableAST");
}

bool ASTValidator::validate(NumberAST* ast)
{
    throw NotImplementedError("Validation of NumberAST");
}

bool ASTValidator::validate(ArrayAST* ast)
{
    throw NotImplementedError("Validation of ArrayAST");
};

bool ASTValidator::validate(CompoundAST* ast)
{
    throw NotImplementedError("Validation of CompoundAST");
}

bool ASTValidator::validate(FunctionAST* ast)
{
    throw NotImplementedError("Validation of FunctionAST");
}

bool ASTValidator::validate(PrototypeAST* ast)
{
    throw NotImplementedError("Validation of PrototypeAST");
}

bool ASTValidator::validate(ReturnAST* ast)
{
    throw NotImplementedError("Validation of ReturnAST");
}

bool ASTValidator::validate(CallAST* ast)
{
    throw NotImplementedError("Validation of CallAST");
}

bool ASTValidator::validate(IfAST* ast)
{
    throw NotImplementedError("Validation of IfAST");
}

bool ASTValidator::validate(ForAST* ast)
{
    throw NotImplementedError("Validation of ForAST");
}

bool ASTValidator::validate(WhileAST* ast)
{
    throw NotImplementedError("Validation of WhileAST");
}
