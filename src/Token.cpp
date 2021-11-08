#include <iostream>
#include "Token.h"
#include "Operator.h"

Token::Token(token_type type, SourceLocation loc)
    : type(type), value(tokenValues.at(type)), loc(loc) {}

Token::Token(token_type type, const std::string& value, SourceLocation loc)
    : type(type), value(value), loc(loc) {}

Operator Token::toOperator() 
{
    // either binary OR unary operator
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
        default:
            return Operator::opUnknown();
    } 
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
