#include <iostream>
#include <sstream>
#include <memory>
#include <string>
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
#include "ASTStringifier.h"

ASTStringifier::ASTStringifier(bool simpleExpr)
    : simpleExpr(simpleExpr) {}

std::string ASTStringifier::toString(ModuleAST* ast, int tabs) 
{
    std::ostringstream s;
    s << "ModuleAST(" << ast->modName << "):\n";
    for (int i = 0; i < ast->children.size(); i++)
    {
        // ModuleAST is always top level, only need to indent once
        s << indent(ast->children[i]->accept(*this, tabs+1), tabs+1);
    }
    return s.str();
}

std::string ASTStringifier::toString(ExpressionAST* ast, int tabs) 
{
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

std::string ASTStringifier::toString(VariableAST* ast, int tabs) 
{
    std::ostringstream s;
    if (simpleExpr)
    {
        s << ast->id;
    }
    else
    {
        s << "VariableAST(" << ast->id << ", " << typeToString(ast->type) << ')';
    }
    return s.str();
}

std::string ASTStringifier::toString(NumberAST* ast, int tabs) 
{
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

std::string ASTStringifier::toString(ArrayAST* ast, int tabs)
{
    std::ostringstream s;
    if (simpleExpr)
    {
        s << '[';
        for (int i = 0; i < ast->elements.size(); i++)
        {
            s << ast->elements[i]->accept(*this);
            if (i < ast->elements.size() - 1)
                s << ", ";
        }
        s << ']';
    }
    else
    {
        s << "ArrayAST(" << ast->elements.size() << ", " << typeToString(ast->type) << "):\n";
        for (int i = 0; i < ast->elements.size(); i++)
        {
            s << indent(ast->elements[i]->accept(*this, tabs+1), tabs+1) << '\n';
        }
    }

    return s.str();
}

std::string ASTStringifier::toString(CompoundAST* ast, int tabs) 
{
    std::ostringstream s;
    if (simpleExpr)
    {
        s << "{ \n";
        for (int i = 0; i < ast->children.size(); i++)
        {
            s << indent(ast->children[i]->accept(*this, tabs+1), tabs+1) << '\n';
        }
        if (ast->retStmt)
            s << indent(ast->retStmt->accept(*this, tabs+1), tabs+1) << '\n';
        s << "}";
    }
    else
    {
        s << "CompoundAST:\n";
        for (int i = 0; i < ast->children.size(); i++)
        {
            s << indent(ast->children[i]->accept(*this, tabs+1), tabs+1);
        }
        if (ast->retStmt)
        {
            if (ast->children.size() > 0)
                s << '\n';
            s << indent(ast->retStmt->accept(*this, tabs+1), tabs+1) << '\n';
        }
    }
    return s.str();
}

std::string ASTStringifier::toString(FunctionAST* ast, int tabs) 
{
    std::ostringstream s;
    if (simpleExpr)
    {
        s << "function:" << ast->proto->accept(*this, tabs);
    }
    else
    {
        s << "FunctionAST:\n";
        s << indent(ast->proto->accept(*this, tabs+1), tabs+1);
        s << indent(ast->body->accept(*this, tabs+1), tabs+1);
    }
    return s.str();
}

std::string ASTStringifier::toString(PrototypeAST* ast, int tabs) 
{
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
        if (ast->params.size() > 0)
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

std::string ASTStringifier::toString(ReturnAST* ast, int tabs)
{
    std::ostringstream s;
    if (simpleExpr)
    {
        if (ast->hasExpr())
            s << "return " << ast->expr->accept(*this) << '\n';
        else
            s << "return void\n";
    }
    else
    {
        s << "ReturnAST:\n";
        if (ast->hasExpr())
            s << indent(ast->expr->accept(*this, tabs+1), tabs+1);
        else
            s << indent("void", tabs+1);
    }
    return s.str();
}

std::string ASTStringifier::toString(CallAST* ast, int tabs)
{
    std::ostringstream s;
    if (simpleExpr)
    {
        s << ast->id << '(';
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
        s << ')';
    }
    else
    {
        s << "CallAST(" << ast->id << ")\n";
        if (ast->params.size() > 0)
            s << indent("Parameters: ", tabs+1) << '\n';
        int i = 0;
        for (auto& param : ast->params)
        {
            s << indent(param->accept(*this, tabs+2), tabs+2);
            if (i < ast->params.size() - 1)
                s << '\n';
            i++;
        }
    }
    return s.str();
}

std::string ASTStringifier::toString(IfAST* ast, int tabs)
{

    std::ostringstream s;
    std::cout << "ATTEMPT TO STRINGIFY IFAST\n";
    if (simpleExpr)
    {
        if (ast->expr)
            s << "if (" << ast->expr->accept(*this) << ") {\n" << ast->body->accept(*this) << "}\n";
        else
            s << "else {\n" << ast->body->accept(*this) << "\n}\n";
    }
    else
    {
        s << "IfAST:\n";
        if (ast->expr)
            s << indent("Expression:\n", tabs+1) << indent(ast->expr->accept(*this, tabs+2), tabs+2);
        if (ast->body)
        {
            if (ast->body && ast->body->children.size() > 0)
            {
                if (ast->expr)
                    s << '\n';
                s << indent("Body:\n", tabs+1) << indent(ast->body->accept(*this, tabs+2), tabs+2);
            }
            if (ast->body->retStmt)
            {
                s << indent(ast->body->retStmt->accept(*this, tabs+2), tabs+2);
            }
        }
        if (ast->other)
        {
            if (ast->body)
                s << '\n';
            s << indent("Other:\n", tabs+1) << indent(ast->other->accept(*this, tabs+2), tabs+2);
        }
    }
    std::cout << "DONE STRINGIFYING IFAST\n";
    return s.str();
}

std::string ASTStringifier::toString(ForAST* ast, int tabs)
{
    std::ostringstream s;
    if (simpleExpr)
    {
        s << "for (" << ast->expr->accept(*this) << " in " << ast->iter->accept(*this) << ") {\n" << ast->body->accept(*this) << "\n}\n";
    }
    else
    {
        s << "ForAST:\n";
        s << indent("Expression:\n", tabs+1) << indent(ast->expr->accept(*this, tabs+2), tabs+2) << '\n';
        s << indent("Iterable:\n", tabs+1) << indent(ast->iter->accept(*this, tabs+2), tabs+2) << '\n';
        if (ast->body->children.size() > 0 || ast->body->retStmt)
        {
            s << indent("Body:\n", tabs+1) << indent(ast->body->accept(*this, tabs+2), tabs+2);
        }
    }
    return s.str();
}

std::string ASTStringifier::toString(WhileAST* ast, int tabs)
{
    std::ostringstream s;
    if (simpleExpr)
    {
        s << "while (" << ast->expr->accept(*this) << ") {\n" << ast->body->accept(*this) << "\n}\n";
    }
    else
    {
        s << "WhileAST:\n";
        if (ast->expr)
            s << indent("Expression:\n", tabs+1) << indent(ast->expr->accept(*this, tabs+2), tabs+2);
        if (ast->body->children.size() > 0 || ast->body->retStmt)
        {
            if (ast->expr)
                s << '\n';
            s << indent("Body:\n", tabs+1) << indent(ast->body->accept(*this, tabs+2), tabs+2);
        }
    }
    return s.str();
}
