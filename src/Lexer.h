#ifndef LEXER_H
#define LEXER_H

#include "io.h"
#include "Token.h"

class Lexer
{
    public:
        Lexer(const std::string& src);
        void debugRead();
        Token peekToken(int offset = 1);
        Token nextToken();

    private:
        const std::string& src;
        int pos;
        char c;
        SourceLocation loc;

        Token lexAlpha();
        Token lexNum();
        Token lexOther();
        Token::token_type lexTokenType();
        bool isKwd(const std::string& s);
        char peek(int offset = 1);
        void next();
        void skipWhitespace();
};

#endif
