#include <iostream>
#include <string>
#include "ASTValidator.h"
#include "../ContextManager.h"
#include "../SymbolTable.h"
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
#include "../ast/UnaryExprAST.h"
#include "../Exception.h"

ParamSymbol::ParamSymbol(const std::string& id, Type type)
    : id(id), type(type) {}

ASTValidator::ASTValidator(std::shared_ptr<ModuleAST> ast, ContextManager& ctx)
    : ast(ast), ctx(ctx), st(Type::eNot), fst({})
{
    ctx.clear();
}

void ASTValidator::validate()
{
    ast->accept(*this);
}

Type ASTValidator::validate(ModuleAST* ast)
{
    ctx.push(ast->fp);
    for (auto& child : ast->children)
    {
        child->accept(*this);
    }
    ctx.pop();
    return Type::eUnd;
}

Type ASTValidator::validate(ExpressionAST* ast)
{
    if (ast->op == Operator::OP_EQL && !ast->getLhs()->isAssignable())
        throw TypeError("expression is not assignable", ast->getLhs()->loc);

    Type lType = ast->getLhs()->accept(*this);
    if (ast->op != Operator::OP_NOP)
    {
      Type rType = ast->getRhs()->accept(*this);

      // check if types are compatible with one another
      if (!typeCompat(lType, rType))
      {
          if (ast->op == Operator::OP_EQL)
              throw TypeError(fmt::format("cannot initialize {} with a value of {}", typeValues[lType], typeValues[rType]), ast->getRhs()->loc);
          else
              throw TypeError(fmt::format("{} and {} are not compatible in binary expression", typeValues[lType], typeValues[rType]), ast->loc);
      }
    }
    return lType;
}

Type ASTValidator::validate(UnaryExprAST* ast)
{
    if (!ast->operand->isAssignable())
        throw TypeError("expression is not assignable", ast->loc);

    return ast->operand->accept(*this);
}

Type ASTValidator::validate(VariableAST* ast)
{
    switch (ast->ctx)
    {
        case VarCtx::eAlloc:
        {
            if (st.contains(ast->id))
            {
                throw ReferenceError(fmt::format("variable '{}' was already defined", ast->id), ast->loc);
            } else {
                st.insert(ast->id, ast->type);
                return ast->type;
            }
        }
        case VarCtx::eParam:
        {
            if (st.contains(ast->id))
              throw ReferenceError(fmt::format("'{}' is a duplicated parameter", ast->id, typeToString(ast->type)), ast->loc);
            st.insert(ast->id, ast->type);
            return ast->type;
        }
        case VarCtx::eReference:
        case VarCtx::eStore:
        {
            if (!st.contains(ast->id))
                throw ReferenceError(fmt::format("reference to unknown variable '{}'", ast->id), ast->loc);
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
    return ast->type;
}

Type ASTValidator::validate(ArrayAST* ast)
{
    throw NotImplementedError("Validation of ArrayAST");
}

Type ASTValidator::validate(CompoundAST* ast)
{
    for (auto& child : ast->children)
        child->accept(*this);
    if (ast->retStmt)
        return ast->retStmt->accept(*this);
    return Type::eVoid;
}

Type ASTValidator::validate(FunctionAST* ast)
{
    Type expectedType = ast->proto->accept(*this);
    // actual return type
    Type actualType = ast->body->accept(*this);
    if (actualType != expectedType)
    {
        if (ast->body->retStmt)
          throw TypeError(fmt::format("returning {} but expected {}", typeValues[actualType], typeValues[expectedType]), ast->body->retStmt->expr->loc);
    }
    st.clear();
    return expectedType;
}

Type ASTValidator::validate(PrototypeAST* ast)
{
    if (fst.contains(ast->name))
        throw ReferenceError(fmt::format("function '{}' was already defined", ast->name), ast->loc);
    ProtoSymbol symb;
    std::vector<ParamSymbol> params;
    for (auto& param : ast->params)
    {
        symb.params.push_back(ParamSymbol(param->id, param->type));
        param->accept(*this);
    }
    symb.retType = ast->retType;
    fst.insert(ast->name, symb);
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
    if (!fst.contains(ast->id))
        throw ReferenceError(fmt::format("reference to unknown function '{}'", ast->id), ast->loc);
    ProtoSymbol symb = fst.lookup(ast->id);
    std::vector<ParamSymbol> expectedTypes = symb.params;
    int numExpected = expectedTypes.size();
    int numReceived = ast->params.size();

    if (numReceived != numExpected)
    {
        SourceLocation errLoc;
        if (numReceived == 0) {
            errLoc = ast->paramsStartLoc;
        } else if (numReceived > numExpected) {
            // underline all extra params
            errLoc = ast->params[numExpected]->loc;
            errLoc.len = (ast->params.back()->loc.x + ast->params.back()->loc.len) - errLoc.x;
        } else {
            // numReceived < numExpected
            // point to len of all params + 1
            errLoc.x = ast->params.back()->loc.x + ast->params.back()->loc.len;
            errLoc.y = ast->params.back()->loc.y;
            errLoc.len = 1;
        }
        
        std::string argumentVerbiage = numExpected == 1 ? "argument" : "arguments";
        std::string expectedVerbiage = numExpected == 0 ? "no" : std::to_string(numExpected);
        std::string receivedVerbiage = numReceived == 0 ? "none" : std::to_string(numReceived);

        throw TypeError(fmt::format("'{}' expects {} {} but received {}", ast->id, expectedVerbiage, argumentVerbiage, receivedVerbiage), errLoc);
    }

    for (int i = 0; i < ast->params.size(); i++)
    {
        Type pType = ast->params[i]->accept(*this);
        if (pType != expectedTypes[i].type)
        {
            throw TypeError(fmt::format("'{}' expects {} argument but received {} for argument '{}'", ast->id, typeValues[expectedTypes[i].type], typeValues[pType], expectedTypes[i].id), ast->params[i]->loc);
        }
    }
    // return return type of function
    return symb.retType;
}

Type ASTValidator::validate(IfAST* ast)
{
    if (ast->expr)
        // could be 'else' statement w/ no expr
        validateBoolExpr(ast->expr);

    return ast->body->accept(*this);
}

Type ASTValidator::validate(ForAST* ast)
{
    throw NotImplementedError("Validation of ForAST");
}

Type ASTValidator::validate(WhileAST* ast)
{
    validateBoolExpr(ast->expr);

    return ast->body->accept(*this);
}

void ASTValidator::validateBoolExpr(std::shared_ptr<ExpressionAST> expr)
{
    Type exprType = expr->accept(*this);
    switch (exprType)
    {
        case Type::eInt:
        case Type::eBool:
            break;
        default:
            throw TypeError(fmt::format("expected boolean expression but got {}", typeValues[exprType]), expr->loc);
    }
}
