#include <iostream>
#include <string>
#include "io.h"
#include "Token.h"
#include "Lexer.h"

Lexer::Lexer(const std::string& src)
    : src(src), pos(0), c(src[pos]), loc({0,1}) {}

void Lexer::debugRead()
{
    while (c != '\0')
    {
        std::cout << (int) c << ' ';
        pos++;
        c = src[pos];
    }
    std::cout << '\n';
}

Token Lexer::peekToken(int offset)
{
    // TODO: handle overflowing past EOF

    // store previous lexer position
    int prevPos = pos;
    char prevChar = c;
    SourceLocation prevLoc = loc;

    // get next token
    Token tok = nextToken();
    if (offset > 1)
    {
        // subtract 1 from offset because first peek is already done above
        for (int i = 0; i < offset-1; i++)
        {
            tok = nextToken();
        }
    }

    // reset lexer position
    pos = prevPos;
    c = prevChar;
    loc = prevLoc;

    return tok;
}

Token Lexer::nextToken()
{
    if (isspace(c))
    {
        skipWhitespace();
    }
    if (isalpha(c))
    {
        return lexAlpha();
    }
    else if (isnumber(c))
    {
        return lexNum();
    }
    else
    {
        return lexOther();
    }
}

Token Lexer::lexAlpha()
{
    std::string s;
    while (isalnum(c))
    {
        s += c;
        next();
    }
    Token::token_type type = isKwd(s) ? Token::TOK_KWD : Token::TOK_ID;
    return Token(type, s, loc);
}

Token Lexer::lexNum()
{
    std::string s;
    while (isalnum(c))
    {
        s += c;
        next();
    }
    return Token(Token::TOK_NUM, s, loc);
}

Token Lexer::lexOther()
{
    Token tok = Token(lexTokenType(), loc);
    next();
    return tok;
}

Token::token_type Lexer::lexTokenType()
{
    skipWhitespace();
    switch(c)
    {
        case '(':
            return Token::TOK_OPAREN;
        case ')':
            return Token::TOK_CPAREN;
        case '{':
            return Token::TOK_OBRACK;
        case '}':
            return Token::TOK_CBRACK;
        case ';':
            return Token::TOK_SCOLON;
        case ':':
            return Token::TOK_COLON;
        case ',':
            return Token::TOK_COMMA;
        case '+':
            return Token::TOK_PLUS;
        case '-':
        {
            if (peek() == '>')
            {
                next();
                return Token::TOK_RARROW;
            }
            return Token::TOK_MINUS;
        }
        case '*':
            return Token::TOK_STAR;
        case '/':
            return Token::TOK_FSLASH;
        case '=':
            return Token::TOK_EQUALS;
        case '\0':
            return Token::TOK_EOF;
        default:
            std::cout << "[Dev Warning] Lexer returning TOK_UND with value of <" << c << "> and (ASCII: " << int(c) << ") (Line " << loc.y << " at position " << loc.x << ")\n";
            next();

            return lexTokenType();
    }
}

bool Lexer::isKwd(const std::string& s)
{
    // reserved keywords
    return (s == "int"
            || s == "return"
            || s == "func");
}

void Lexer::next()
{
    pos++;
    loc.x++;
    c = src[pos];
}

char Lexer::peek(int offset)
{
    return src[pos+1];
}

void Lexer::skipWhitespace()
{
    while (isspace(c))
    {
        if (c == '\n')
        {
            loc.y++;
            loc.x = 0;
        }
        next();
    }
}
