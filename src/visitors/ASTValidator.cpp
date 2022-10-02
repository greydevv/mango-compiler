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
    : ast(ast), ctx(ctx), st(Type::eNot), fst({}), expectedTy(Type::eUnd)
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

    Type prevExpectedTy = expectedTy;
    expectedTy = lType;
    if (ast->op != Operator::OP_NOP)
    {
      Type rType = ast->getRhs()->accept(*this);

      if (dynamic_cast<NumberAST*>(ast->getLhs().get()) != nullptr && dynamic_cast<NumberAST*>(ast->getRhs().get()) != nullptr)
      {
        return lType;
      }

      // check if types are compatible with one another
      if (!typeCompat(lType, rType))
      {
          if (ast->op == Operator::OP_EQL)
              throw TypeError(fmt::format("cannot initialize {} with a value of {}", typeValues[lType], typeValues[rType]), ast->getRhs()->loc);
          else
              throw TypeError(fmt::format("{} and {} are not compatible in binary expression", typeValues[lType], typeValues[rType]), ast->loc);
      }
      switch (ast->op)
      {
          case Operator::OP_BOOL_GT:
          case Operator::OP_BOOL_LT:
          case Operator::OP_BOOL_GTE:
          case Operator::OP_BOOL_LTE:
          case Operator::OP_BOOL_EQL:
          case Operator::OP_BOOL_NEQL:
          case Operator::OP_BOOL_OR:
          case Operator::OP_BOOL_AND:
              {
                  expectedTy = prevExpectedTy;
                  return Type::eBool;
              }
          default: 
              break;
      }
    }
    expectedTy = prevExpectedTy;
    return lType;
}

Type ASTValidator::validate(UnaryExprAST* ast)
{
    if (!ast->operand->isAssignable())
        throw TypeError("expression is not assignable", ast->operand->loc);

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
    if (ast->type == Type::eBool)
      return Type::eBool;
    
    switch (expectedTy)
    {
        case Type::eUnd:
            return ast->type;
        case Type::eBool:
        case Type::eInt8:
            if (ast->val <= INT8_MAX && ast->val >= INT8_MIN)
            {
                ast->type = Type::eInt8;
                break;
            }
        case Type::eUInt8:
            if (ast->val <= UINT8_MAX && ast->val >= 0)
            {
                ast->type = Type::eUInt8;
                break;
            }
        case Type::eInt16:
            if (ast->val <= INT16_MAX && ast->val >= INT16_MIN)
            {
                ast->type = Type::eInt16;
                break;
            }
        case Type::eUInt16:
            if (ast->val <= UINT16_MAX && ast->val >= 0)
            {
                ast->type = Type::eUInt16;
                break;
            }
        case Type::eInt32:
            if (ast->val <= INT32_MAX && ast->val >= INT32_MIN)
            {
                ast->type = Type::eInt32;
                break;
            }
        case Type::eUInt32:
            if (ast->val <= UINT32_MAX && ast->val >= 0)
            {
                ast->type = Type::eUInt32;
                break;
            }
        case Type::eInt64:
            if (ast->val <= (long double) INT64_MAX && ast->val >= INT64_MIN)
            {
                ast->type = Type::eInt64;
                break;
            }
        case Type::eUInt64:
            if (ast->val <= (long double) UINT64_MAX && ast->val >= 0)
            {
                ast->type = Type::eUInt64;
                break;
            }
        case Type::eInt128:
            if (ast->val <= INT128_MAX && ast->val >= INT128_MIN)
            {
                ast->type = Type::eInt128;
                break;
            }
        case Type::eUInt128:
            if (ast->val <= UINT128_MAX && ast->val >= 0)
            {
                ast->type = Type::eUInt128;
                break;
            }
        default:
          throw TypeError(fmt::format("numeric literal too large"), ast->loc);
    }

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
    Type expectedRetTy = ast->proto->accept(*this);
    Type prevExpectedTy = expectedTy;
    expectedTy = expectedRetTy;
    // actual return type
    Type actualRetTy = ast->body->accept(*this);
    if (actualRetTy != expectedRetTy)
    {
        if (ast->body->retStmt)
          throw TypeError(fmt::format("returning {} but expected {}", typeValues[actualRetTy], typeValues[expectedRetTy]), ast->body->retStmt->expr->loc);
    }
    st.clear();
    expectedTy = prevExpectedTy;
    return expectedRetTy;
}

Type ASTValidator::validate(PrototypeAST* ast)
{
    if (fst.contains(ast->name))
        throw ReferenceError(fmt::format("function '{}' was already defined", ast->name), ast->loc);
    ProtoSymbol symb;
    for (auto& param : ast->params)
    {
        symb.params.push_back(ParamSymbol(param->id, param->type));
        param->accept(*this);
    }
    symb.retType = ast->retType;
    if (ast->isExtern)
      // clear the symbol table if it's an extern statement
      st.clear();
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

    Type prevExpectedTy = expectedTy;
    for (int i = 0; i < ast->params.size(); i++)
    {
        expectedTy = expectedTypes[i].type;
        Type pType = ast->params[i]->accept(*this);
        if (pType != expectedTypes[i].type)
        {
            throw TypeError(fmt::format("'{}' expects {} for argument '{}' but received {}", ast->id, typeValues[expectedTypes[i].type], expectedTypes[i].id, typeValues[pType]), ast->params[i]->loc);
        }
    }
    expectedTy = prevExpectedTy;
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
    expectedTy = Type::eBool;
    Type exprType = expr->accept(*this);
    switch (exprType)
    {
        case Type::eBool:
            break;
        default:
            throw TypeError(fmt::format("expected boolean expression but got {}", typeValues[exprType]), expr->loc);
    }
}
