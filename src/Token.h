#ifndef TOKEN_H
#define TOKEN_H

#include <sstream>
#include <map>
#include "Operator.h"
#include "Types.h"

struct SourceLocation
{
    int x; // line
    int y; // character on line

    SourceLocation(int x, int y);
};

class Token final
{
    public:
        enum token_type
        {
            TOK_NUM,
            TOK_ID,
            TOK_KWD,
            TOK_TYPE,
            TOK_OPAREN,
            TOK_CPAREN,
            TOK_OBRACK,
            TOK_CBRACK,
            TOK_RARROW,
            TOK_EQUALS,
            TOK_COMMA,
            TOK_SCOLON,
            TOK_COLON,
            TOK_PLUS,
            TOK_MINUS,
            TOK_STAR,
            TOK_FSLASH,
            TOK_EOF,
            TOK_UND,
        };

        token_type type;
        std::string value;
        SourceLocation loc;
        
        Token(token_type type, SourceLocation loc);
        Token(token_type type, const std::string& value, SourceLocation loc);
        Operator toOperator();
        Type toType();
        bool isType();
        static token_type typeFromChar(char c);
        bool operator==(token_type other_type);
        bool operator!=(token_type other_type);
        friend std::ostream& operator<<(std::ostream& os, const Token& tok);
};

static const std::string tokenStrings[] = {
    "TOK_NUM",
    "TOK_ID",
    "TOK_KWD",
    "TOK_TYPE",
    "TOK_OPAREN",
    "TOK_CPAREN",
    "TOK_OBRACK",
    "TOK_CBRACK",
    "TOK_RARROW",
    "TOK_EQUALS",
    "TOK_COMMA",
    "TOK_SCOLON",
    "TOK_COLON",
    "TOK_PLUS",
    "TOK_MINUS",
    "TOK_STAR",
    "TOK_FSLASH",
    "TOK_EOF",
    "TOK_UND",
};

static const std::map<Token::token_type, std::string> tokenValues = {
    {Token::TOK_NUM, "number"},
    {Token::TOK_ID, "identifier"},
    {Token::TOK_KWD, "kwd"},
    {Token::TOK_TYPE, ""},
    {Token::TOK_OPAREN, "("},
    {Token::TOK_CPAREN, ")"},
    {Token::TOK_OBRACK, "{"},
    {Token::TOK_CBRACK, "}"},
    {Token::TOK_RARROW, "->"},
    {Token::TOK_EQUALS, "="},
    {Token::TOK_COMMA, ","},
    {Token::TOK_SCOLON, ";"},
    {Token::TOK_COLON, ":"},
    {Token::TOK_PLUS, "+"},
    {Token::TOK_MINUS, "-"},
    {Token::TOK_STAR, "*"},
    {Token::TOK_FSLASH, "/"},
    {Token::TOK_EOF, "\0"},
    {Token::TOK_UND, ""}
};

#endif
