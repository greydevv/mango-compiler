#include <iostream>
#include "Token.h"
#include "Operator.h"
#include "Types.h"

FileLocation::FileLocation()
    : x(1), y(1) {}

FileLocation::FileLocation(int x, int y)
    : x(x), y(y) {}

SourceLocation::SourceLocation(int x, int y, int len)
    : x(x), y(y), len(len) {}

SourceLocation::SourceLocation(const FileLocation& floc, int len)
    : x(floc.x), y(floc.y), len(len) {}

Token::Token(token_type type, SourceLocation loc)
    : type(type), value(tokenValues.at(type)), loc(loc) {}

Token::Token(token_type type, const std::string& value, SourceLocation loc)
    : type(type), value(value), loc(loc) {}

Operator Token::toOperator() 
{
    switch (type)
    {
        case Token::TOK_EQUALS:
            return Operator::opEqual();
        case Token::TOK_PLUS:
            return Operator::opAdd();
        case Token::TOK_MINUS:
            return Operator::opSubtract();
        case Token::TOK_STAR:
            return Operator::opMultiply();
        case Token::TOK_FSLASH:
            return Operator::opDivide();
        case Token::TOK_NEQUALS:
            return Operator::opNotEqualTo();
        case Token::TOK_DEQUALS:
            return Operator::opEqualTo();
        case Token::TOK_LT:
            return Operator::opLT();
        case Token::TOK_LTE:
            return Operator::opLTE();
        case Token::TOK_GT:
            return Operator::opGT();
        case Token::TOK_GTE:
            return Operator::opGTE();
        case Token::TOK_DAMPSND:
            return Operator::opLogicalAnd();
        case Token::TOK_DBAR:
            return Operator::opLogicalOr();
        default:
            return Operator::opUnknown();
    } 
}

Type Token::toType()
{
    if (value == "void")
    {
        return Type::eVoid;
    }
    else if (value == "int")
    {
        return Type::eInt;
    }
    else
    {
        return Type::eUnd;
    }
}

bool Token::isType()
{
    return (value == "int"
            || value == "return"
            || value == "func");
}

Token::token_type Token::typeFromChar(char c)
{
    switch(c)
    {
        case '(':
            return Token::TOK_OPAREN;
        case ')':
            return Token::TOK_CPAREN;
        case ':':
            return Token::TOK_COLON;
        case ',':
            return Token::TOK_COMMA;
        case '+':
            return Token::TOK_PLUS;
        case '-':
            return Token::TOK_MINUS;
        case '*':
            return Token::TOK_STAR;
        case '/':
            return Token::TOK_FSLASH;
        default:
            return Token::TOK_UND;
    }
}

std::ostream& operator<<(std::ostream& os, const Token& tok)
{
    if (tok.type == Token::TOK_EOF)
    {
        os << '<' << tokenStrings[tok.type] << " (line: " << tok.loc.y << ": " << tok.loc.x << ")>";
    }
    else
    {
        os << '<' << tokenStrings[tok.type] << ": " << tok.value << " (line: " << tok.loc.y << ": " << tok.loc.x << ")>";
    }
    return os;
}

bool Token::operator==(token_type other_type)
{
    return type == other_type;
}

bool Token::operator!=(token_type other_type)
{
    return type != other_type;
}
