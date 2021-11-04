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

std::string ASTStringifier::toString(ExpressionAST* ast, int tabs) {
    std::ostringstream s;
    s << "ExpressionAST(" << operatorValues.at(ast->op) << "):\n";
    s << indent(ast->LHS->accept(*this, tabs+1), tabs+1) << '\n';
    s << indent(ast->RHS->accept(*this, tabs+1), tabs+1);
    return s.str();
}

std::string ASTStringifier::toString(ModuleAST* ast, int tabs) {
    std::ostringstream s;
    s << "ModuleAST:\n";
    for (int i = 0; i < ast->children.size(); i++)
    {
        s << indent(ast->children[i]->accept(*this, 1), 1) << '\n';
    }
    return s.str();
}

std::string ASTStringifier::toString(NumberAST* ast, int tabs) {
    std::ostringstream s;
    s << "NumberAST(" << ast->val << ')';
    return s.str();
}

std::string ASTStringifier::toString(VariableAST* ast, int tabs) {
    std::ostringstream s;
    s << "VariableAST(" << ast->id << ')';
    return s.str();
}

std::string ASTStringifier::indent(std::string str, int tabs)
{
    return str.insert(0, tabs*2, ' ');
}