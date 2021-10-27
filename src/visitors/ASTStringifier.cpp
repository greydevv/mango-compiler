#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include "../ast/AST.h"
#include "../ast/ExpressionAST.h"
#include "../ast/ModuleAST.h"
#include "../ast/VariableAST.h"
#include "../ast/NumberAST.h"
#include "ASTStringifier.h"

std::string ASTStringifier::toString(ExpressionAST* ast) {
    std::ostringstream s;
    s << "ExpressionAST:\n\t";
    s << "LHS: " << ast->LHS->accept(*this) << "\n\t";
    s << "RHS: " << ast->RHS->accept(*this) << '\n';
    return s.str();
}

std::string ASTStringifier::toString(ModuleAST* ast) {
    std::ostringstream s;
    s << "ModuleAST:\n\t";
    for (int i = 0; i < ast->children.size(); i++)
    {
        s << ast->children[i]->accept(*this) << '\n';
    }
    return s.str();
}

std::string ASTStringifier::toString(NumberAST* ast) {
    std::ostringstream s;
    s << "NumberAST(" << ast->val << ')';
    return s.str();
}

std::string ASTStringifier::toString(VariableAST* ast) {
    std::ostringstream s;
    s << "VariableAST(" << ast->id << ')';
    return s.str();
}