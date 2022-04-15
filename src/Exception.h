#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include "ContextManager.h"
#include "Token.h"

class BaseException : public std::exception
{
    public:
        BaseException(const std::string& msg);
        virtual std::string getMsg(const ContextManager& ctx) const;
        virtual std::string getExcName() const = 0;
        virtual const char* what() const throw();
    protected:
        const std::string msg;
};

class BaseSourceException : public BaseException
{
    public:
        BaseSourceException(const std::string& msg, const std::string& line, SourceLocation loc);
        virtual std::string getMsg(const ContextManager& ctx) const override;
        virtual std::string getExcName() const override = 0;
    protected:
        std::string fname;
        const std::string line;
        const SourceLocation loc;
};

class CompilationError : public BaseException
{
    public:
        CompilationError(const std::string& msg);
        virtual std::string getExcName() const override;
    private:
        const std::string msg;
};

class NotImplementedError : public BaseException
{
    public:
        NotImplementedError(const std::string& msg);
        virtual std::string getExcName() const override;
};

class FileNotFoundError : public BaseSourceException
{
    public:
        FileNotFoundError(const std::string& badFname, const std::string& line, SourceLocation loc);
        virtual std::string getExcName() const override;
};

class SyntaxError : public BaseSourceException
{
    public:
        SyntaxError(const std::string& msg, const std::string& line, SourceLocation loc);
        virtual std::string getExcName() const override;
};

class ReferenceError : public BaseSourceException
{
    public:
        ReferenceError(const std::string& msg, const std::string& line, SourceLocation loc);
        virtual std::string getExcName() const override;
};

class TypeError : public BaseSourceException
{
    public:
        TypeError(const std::string& msg, const std::string& line, SourceLocation loc);
        virtual std::string getExcName() const override;
};

std::string getFileNameFromPath(const std::string& fullPath);

#endif
