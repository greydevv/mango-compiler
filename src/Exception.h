#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include "Token.h"

class BaseException : public std::exception
{
    public:
        BaseException(const std::string& excName, const std::string& fname, const std::string& msg, SourceLocation loc);
        std::ostringstream getErrMetaStream() const;
        virtual std::string getMsg() const = 0;
        virtual const char* what() const throw();

    protected:
        const std::string excName;
        const std::string fname;
        const std::string msg;
        const SourceLocation loc;
};

class SyntaxError : public BaseException
{
    public:
        SyntaxError(const std::string& fname, const std::string& msg, const std::string& line, SourceLocation loc);
        virtual std::string getMsg() const override;
    private:
        const std::string line;
};

class ReferenceError : public BaseException
{
    public:
        ReferenceError(const std::string& fname, const std::string& msg, const std::string& line, SourceLocation loc);
        virtual std::string getMsg() const override;
    private:
        const std::string line;
};

class NotImplementedError : public BaseException
{
    public:
        NotImplementedError(const std::string& fname, const std::string& msg, SourceLocation loc);
        virtual std::string getMsg() const override;
};

std::string getFileNameFromPath(const std::string& fullPath);

#endif
