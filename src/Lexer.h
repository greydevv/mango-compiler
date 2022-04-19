#ifndef LEXER_H
#define LEXER_H

#include "io.h"
#include "Token.h"

class Lexer
{
    public:
        Lexer(const FilePath& fp, ContextManager& ctx);
        Token peekToken();
        Token nextToken();
        std::string getLine(int lineNo);

    private:
        std::ifstream is;
        char c;
        FileLocation loc;

        Token lexAlpha();
        Token lexNum();
        Token lexString();
        Token lexOther();
        Token::token_type lexTokenType();
        bool isKwd(const std::string& s);
        bool isType(const std::string& s);
        char peek();
        void next();
        void skipWhitespace();
        void skipComment();
};

#endif
