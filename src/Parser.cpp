#include <iostream>
#include <fstream>
#include <vector>
#include "Token.h"
#include "Operator.h"
#include "Lexer.h"
#include "Parser.h"
#include "ast/ModuleAST.h"
#include "ast/ExpressionAST.h"
#include "ast/VariableAST.h"
#include "ast/NumberAST.h"

Parser::Parser(const std::string& src)
    : lexer(Lexer(src)), tok(Token::TOK_EOF, {0,0})
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
        // could be either a funcdef, control flow, or variable statement
        case Token::TOK_KWD:
            return parseKwd();
        default:
            std::unique_ptr<AST> exprAST = parseExpr();
            eat(Token::TOK_SCOLON);
            return exprAST;
    }
}

std::unique_ptr<AST> Parser::parseKwd()
{
    // assuming it's a variable declaration for now
    eat(Token::TOK_KWD);
    std::unique_ptr<AST> exprAST = parseExpr();
    eat(Token::TOK_SCOLON);
    return exprAST;
}

std::unique_ptr<NumberAST> Parser::parseNum() 
{
    std::unique_ptr<NumberAST> numAST = std::make_unique<NumberAST>(std::stod(tok.value));
    eat(Token::TOK_NUM);
    return numAST;
}

std::unique_ptr<VariableAST> Parser::parseId() 
{
    std::unique_ptr<VariableAST> varAST = std::make_unique<VariableAST>(tok.value);
    std::cout << "parseId(" << varAST->id << ")\n";
    eat(Token::TOK_ID);
    return varAST;
}

std::unique_ptr<AST> Parser::parseTerm() 
{
    switch (tok.type)
    {
        case Token::TOK_NUM:
            return parseNum();
        case Token::TOK_ID:
            return parseId();
        default:
            setErrState(1);
            std::cout << "[Dev Error] Not an operand:  " << tok << '\n';
            eat(tok.type);
            exit(1);
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
        std::cout << nextOp << " >= " << prec << '\n';
        Operator currOp = nextOp;
        eat();
        std::unique_ptr<AST> R = parseTerm();
        nextOp = tok.toOperator();
        while ((nextOp > currOp) || ((currOp == nextOp) && (nextOp.getAssoc() == Operator::A_RIGHT)))
        {
            // if (nextOp > currOp)
            // {
            //     std::cout << operatorStrings[nextOp.getType()] << " > " << operatorStrings[currOp.getType()] << '\n';
            // }
            // else 
            // {
            //     std::cout << operatorStrings[nextOp.getType()] << " == " << operatorStrings[currOp.getType()] << '\n';
            // }
            auto RHS = std::unique_ptr<AST>(R->clone());
            int nextPrec = currOp.getType() == Operator::OP_EXP ? currOp.getPrec() : currOp.getPrec()+1;
            R = parseSubExpr(std::move(RHS), nextPrec);
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
        std::cout << "[Dev Warning] Unexpected token: got " << tokenStrings[tok.type] << " but expected " << tokenStrings[expectedType] << '\n';
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

int Parser::getErrState() {
    return errState;
}

void Parser::setErrState(int errState) {
    this->errState = errState;
}