#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include "Token.h"

class BaseException : public std::exception
{
    public:
        BaseException(const std::string& msg, SourceLocation loc);
        virtual std::string buildMsg() const = 0;
        virtual const char* what() const throw();

    protected:
        std::string msg;
        SourceLocation loc;
};

class SyntaxError : public BaseException
{
    public:
        SyntaxError(const std::string& msg, SourceLocation loc);
        virtual std::string buildMsg() const override;
};

class ReferenceError : public BaseException
{
    public:
        ReferenceError(const std::string& msg, SourceLocation loc);
        virtual std::string buildMsg() const override;
};

class NotImplementedError : public BaseException
{
    public:
        NotImplementedError(const std::string& msg, SourceLocation loc);
        virtual std::string buildMsg() const override;
};

#endif
