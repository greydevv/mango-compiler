#include <iostream>
#include <fstream>
#include <memory>
#include <system_error>
#include <vector>
#include "Token.h"
#include "Operator.h"
#include "Types.h"
#include "Lexer.h"
#include "Parser.h"
#include "io.h"
#include "ast/ModuleAST.h"
#include "ast/ExpressionAST.h"
#include "ast/VariableAST.h"
#include "ast/NumberAST.h"
#include "ast/FunctionAST.h"
#include "ast/PrototypeAST.h"
#include "ast/ReturnAST.h"
#include "ast/CallAST.h"
#include "ast/IfAST.h"
#include "Exception.h"

Parser::Parser(const std::string& fname, const std::string& src)
    : fname(fname), lexer(Lexer(src)), tok(Token::TOK_EOF, {0,0})
{
    tok = lexer.nextToken();
}

std::unique_ptr<ModuleAST> Parser::parse()
{
    std::unique_ptr<ModuleAST> modAST = std::make_unique<ModuleAST>();
    while (tok.type != Token::TOK_EOF)
    {
        modAST->addChild(parsePrimary());
    }
    return modAST;
}

std::unique_ptr<AST> Parser::parsePrimary() 
{
    switch (tok.type)
    {
        case Token::TOK_KWD:
            return parseKwd();
        case Token::TOK_ID:
            // either variable redefinition, reference
            return parseId();
        case Token::TOK_TYPE:
        {
            // assuming variable declaration
            // get type of variable declaration
            Type allocType = typeFromString(tok.value);
            eat(Token::TOK_TYPE);
            auto allocVar = std::make_unique<VariableAST>(tok.value, allocType, VarCtx::eAlloc);
            eat(Token::TOK_ID);
            eat(Token::TOK_EQUALS);
            auto allocExpr = std::make_unique<ExpressionAST>(std::move(allocVar), parseExpr(), Operator::OP_EQL);
            eat(Token::TOK_SCOLON);
            return allocExpr;
        }
        default:
            std::unique_ptr<AST> exprAST = parseExpr();
            eat(Token::TOK_SCOLON);
            return exprAST;
    }
}

std::unique_ptr<AST> Parser::parseKwd()
{
    if (tok.value == "return")
    {
        return parseReturnStmt();
    }
    else if (tok.value == "func")
    {
        return parseFuncDef();
    }
    else if (tok.value == "if")
    {
        return parseIfStmt();
    }
    else
    {
        std::cout << "[Dev Error] Keyword handling for '" << tok.value << "' not yet implemented in parser.\n";
        exit(1);
    }
}

std::unique_ptr<AST> Parser::parseId()
{
    if (lexer.peekToken().type == Token::TOK_EQUALS)
    {
        auto var = std::make_unique<VariableAST>(tok.value, Type::eUnd, VarCtx::eStore);
        eat(Token::TOK_ID);
        eat(Token::TOK_EQUALS);
        auto expr = std::make_unique<ExpressionAST>(std::move(var), parseExpr(), Operator::OP_EQL); 
        eat(Token::TOK_SCOLON);
        return expr;
    }
    std::unique_ptr<AST> expr = parseExpr();
    eat(Token::TOK_SCOLON);
    return expr;
}

std::unique_ptr<FunctionAST> Parser::parseFuncDef()
{
    eat(Token::TOK_KWD);
    return std::make_unique<FunctionAST>(parseFuncProto(), parseCompound());
}

std::unique_ptr<PrototypeAST> Parser::parseFuncProto()
{
    std::string name = tok.value;
    eat(Token::TOK_ID);
    std::vector<std::unique_ptr<VariableAST>> params = parseFuncParams();
    Type retType = Type::eVoid;
    if (tok == Token::TOK_RARROW)
    {
        eat(Token::TOK_RARROW);
        retType = tok.toType();
        eat(Token::TOK_TYPE);
    }
    return std::make_unique<PrototypeAST>(name, retType, std::move(params));
}

std::vector<std::unique_ptr<VariableAST>> Parser::parseFuncParams()
{
    // TODO: refactor this and parseFuncParams into a method like parseCsv
    std::vector<std::unique_ptr<VariableAST>> params;
    eat(Token::TOK_OPAREN);
    if (tok.type != Token::TOK_CPAREN)
    {
        while (true)
        {
            Type paramType = typeFromString(tok.value);
            eat(Token::TOK_TYPE);
            auto param = std::make_unique<VariableAST>(tok.value, paramType, VarCtx::eParam);
            params.push_back(std::move(param));
            eat(Token::TOK_ID);
            if (tok == Token::TOK_COMMA)
            {
                eat(Token::TOK_COMMA);
            }
            else
            {
                break;
            }
        }
    }
    eat(Token::TOK_CPAREN);
    return params;
}

std::vector<std::unique_ptr<AST>> Parser::parseCallParams()
{
    // TODO: refactor this and parseFuncParams into a method like parseCsv
    std::vector<std::unique_ptr<AST>> params;
    eat(Token::TOK_OPAREN);
    if (tok.type != Token::TOK_CPAREN)
    {
        while (true)
        {
            std::unique_ptr<AST> param = parseExpr();
            params.push_back(std::move(param));
            if (tok == Token::TOK_COMMA)
            {
                eat(Token::TOK_COMMA);
            }
            else
            {
                break;
            }
        }
    }
    eat(Token::TOK_CPAREN);
    return params;
}

std::unique_ptr<CompoundAST> Parser::parseCompound()
{
    eat(Token::TOK_OBRACK);
    std::vector<std::unique_ptr<AST>> children;
    while (tok != Token::TOK_CBRACK)
    {
        children.push_back(parsePrimary());
    }
    eat(Token::TOK_CBRACK);
    return std::make_unique<CompoundAST>(std::move(children));
}

std::unique_ptr<ReturnAST> Parser::parseReturnStmt()
{
    eat(Token::TOK_KWD);
    std::unique_ptr<AST> expr = parseExpr();
    eat(Token::TOK_SCOLON);
    return std::make_unique<ReturnAST>(std::move(expr));
}

std::unique_ptr<IfAST> Parser::parseIfStmt()
{
    eat(Token::TOK_KWD);
    eat(Token::TOK_OPAREN);
    if (tok.type == Token::TOK_CPAREN)
    {
        std::ostringstream s;
        s << "expected expression\n";
        s << underlineError(lexer.getLine(tok.loc.y), tok.loc.x, tok.value.size());
        throw SyntaxError(fname, s.str(), tok.loc);
    }
    std::unique_ptr<AST> expr = parseExpr();
    eat(Token::TOK_CPAREN);
    std::unique_ptr<CompoundAST> body = parseCompound();
    std::unique_ptr<IfAST> other = nullptr;
    if (tok.type == Token::TOK_KWD && tok.value == "else")
    {
        eat();
        if (tok.type == Token::TOK_KWD && tok.value == "if")
            // parse 'else if'
            other = parseIfStmt();
        else
            // parse 'else' (neither expr nor other should be present)
            other = std::make_unique<IfAST>(nullptr, parseCompound(), nullptr);
    }
    return std::make_unique<IfAST>(std::move(expr), std::move(body), std::move(other));
}

std::unique_ptr<NumberAST> Parser::parseNum() 
{
    std::unique_ptr<NumberAST> numAST = std::make_unique<NumberAST>(std::stod(tok.value));
    eat(Token::TOK_NUM);
    return numAST;
}

std::unique_ptr<AST> Parser::parseIdTerm() 
{
    std::string id = tok.value;
    eat(Token::TOK_ID);
    // check if function call
    if (tok.type == Token::TOK_OPAREN)
    {
        auto callAST = std::make_unique<CallAST>(id, parseCallParams());
        return callAST;
    }
    auto varAST = std::make_unique<VariableAST>(id);
    return varAST;
}

std::unique_ptr<AST> Parser::parseTerm() 
{
    switch (tok.type)
    {
        case Token::TOK_NUM:
            return parseNum();
        case Token::TOK_ID:
            return parseIdTerm();
        case Token::TOK_OPAREN:
            {
                eat(Token::TOK_OPAREN);
                std::unique_ptr<AST> expr = parseExpr();
                eat(Token::TOK_CPAREN);
                return expr;
            }
        case Token::TOK_SCOLON:
            {
                // specific error message
                std::ostringstream s;
                s << "expected expression\n";
                s << underlineError(lexer.getLine(tok.loc.y), tok.loc.x, tok.value.size());
                throw SyntaxError(fname, s.str(), tok.loc);
            }
        default:
            {
                std::ostringstream s;
                s << '\'' << tok.value << "' is not a valid operand\n";
                s << underlineError(lexer.getLine(tok.loc.y), tok.loc.x, tok.value.size());
                throw SyntaxError(fname, s.str(), tok.loc);
            }
    }
}

std::unique_ptr<AST> Parser::parseExpr()
{  
    return parseSubExpr(parseTerm());
}

std::unique_ptr<AST> Parser::parseSubExpr(std::unique_ptr<AST> L, int prec)
{
    Operator nextOp = tok.toOperator();
    while (nextOp >= prec)
    {
        if (nextOp.getType() == Operator::OP_EQL)
        {
            std::ostringstream s;
            s << "expression is not assignable\n";
            s << underlineError(lexer.getLine(tok.loc.y), tok.loc.x, tok.value.size());
            throw SyntaxError(fname, s.str(), tok.loc);
        }
        Operator currOp = nextOp;
        eat();
        std::unique_ptr<AST> R = parseTerm();
        nextOp = tok.toOperator();
        while ((nextOp > currOp) || ((currOp == nextOp) && (nextOp.getAssoc() == Operator::A_RIGHT)))
        {
            if (nextOp.getType() == Operator::OP_EQL)
            {
                std::ostringstream s;
                s << "expression is not assignable\n";
                s << underlineError(lexer.getLine(tok.loc.y), tok.loc.x, tok.value.size());
                throw SyntaxError(fname, s.str(), tok.loc);
            }
            auto RHS = std::unique_ptr<AST>(R->clone());
            if (currOp.getAssoc() == Operator::A_RIGHT)
                R = parseSubExpr(std::move(RHS), currOp.getPrec());
            else
                R = parseSubExpr(std::move(RHS), currOp.getPrec()+1);
            nextOp = tok.toOperator();
        }
        auto LHS = std::unique_ptr<AST>(L->clone());
        L = std::make_unique<ExpressionAST>(std::move(LHS), std::move(R), currOp.getType());
    }
    return L;
}

bool Parser::eat(Token::token_type expectedType)
{
    bool badToken = (tok.type != expectedType);
    if (badToken)
    {
        setErrState(1);
        std::ostringstream s;
        s << "expected '" << tokenValues.at(expectedType) << "' but got '" << tok.value << "' instead\n"; 
        s << underlineError(lexer.getLine(tok.loc.y), tok.loc.x, tok.value.size());
        throw SyntaxError(fname, s.str(), tok.loc);
    }
    getToken();
    return badToken;
}

bool Parser::eat()
{
    // this method is for eating current token no matter its type
    return eat(tok.type);
}

void Parser::getToken()
{
    tok = lexer.nextToken();
}

int Parser::getErrState()
{
    return errState;
}

void Parser::setErrState(int errState) 
{
    this->errState = errState;
}
