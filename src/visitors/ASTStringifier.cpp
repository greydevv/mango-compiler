#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include "../ast/AST.h"
#include "../ast/ModuleAST.h"
#include "../ast/ExpressionAST.h"
#include "../ast/VariableAST.h"
#include "../ast/NumberAST.h"
#include "../ast/CompoundAST.h"
#include "../ast/FunctionAST.h"
#include "../ast/PrototypeAST.h"
#include "ASTStringifier.h"

ASTStringifier::ASTStringifier(bool simpleExpr)
    : simpleExpr(simpleExpr) {}

std::string ASTStringifier::toString(ModuleAST* ast, int tabs) {
    std::ostringstream s;
    s << "ModuleAST:\n";
    for (int i = 0; i < ast->children.size(); i++)
    {
        // ModuleAST is always top level, only need to indent once
        s << indent(ast->children[i]->accept(*this, 1), 1) << '\n';
    }
    return s.str();
}

std::string ASTStringifier::toString(ExpressionAST* ast, int tabs) {
    std::ostringstream s;
    if (simpleExpr)
    {
        s << '(' << ast->LHS->accept(*this, tabs);
        s << ' ' << operatorValues.at(ast->op) << ' ';
        s << ast->RHS->accept(*this, tabs) << ')';
    }
    else
    {
        s << "ExpressionAST(" << operatorValues.at(ast->op) << "):\n";
        s << indent(ast->LHS->accept(*this, tabs+1), tabs+1) << '\n';
        s << indent(ast->RHS->accept(*this, tabs+1), tabs+1);
    }
    return s.str();
}

std::string ASTStringifier::toString(NumberAST* ast, int tabs) {
    std::ostringstream s;
    if (simpleExpr)
    {
        s << ast->val;
    }
    else
    {
        s << "NumberAST(" << ast->val << ')';
    }
    return s.str();
}

std::string ASTStringifier::toString(VariableAST* ast, int tabs) {
    std::ostringstream s;
    if (simpleExpr)
    {
        s << ast->id;
    }
    else
    {
        s << "VariableAST(" << ast->id << ')';
    }
    return s.str();
}

std::string ASTStringifier::toString(CompoundAST* ast, int tabs) {
    return "CompoundAST (Not Implemented)";
}

std::string ASTStringifier::toString(FunctionAST* ast, int tabs) {
    std::ostringstream s;
    if (simpleExpr)
    {
        s << "function:" << ast->proto->accept(*this, tabs);
    }
    else
    {
        s << "FunctionAST:\n";
        s << indent(ast->proto->accept(*this, tabs+1), tabs+1);
    }
    return s.str();
}

std::string ASTStringifier::toString(PrototypeAST* ast, int tabs) {
    std::ostringstream s;
    if (simpleExpr)
    {
        s << ast->name << '(';
        int i = 0;
        for (auto& param : ast->params)
        {
            s << param->accept(*this, tabs);
            if (i < ast->params.size() - 1)
            {
                s << ", ";
            }
            i++;
        }
        s << ") returns " << typeToString(ast->retType);
    }
    else
    {
        s << "PrototypeAST(" << ast->name << ")\n";
        s << indent("Returns: ", tabs+1) << typeToString(ast->retType) << '\n';
        s << indent("Parameters: ", tabs+1) << '\n';
        for (auto& param : ast->params)
        {
            s << indent(param->accept(*this, tabs+2), tabs+2) << '\n';
        }
    }
    return s.str();
}

std::string ASTStringifier::indent(std::string str, int tabs)
{
    return str.insert(0, tabs*2, ' ');
}
