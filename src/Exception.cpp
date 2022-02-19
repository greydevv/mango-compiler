#include <string>
#include "Exception.h"
#include "Token.h"

BaseException::BaseException(const std::string& msg, SourceLocation loc)
    : msg(msg), loc(loc) {}

const char* BaseException::what() const throw()
{
    std::string what = buildMsg();
    const char* what_cstr = what.c_str();
    return what_cstr;
}

SyntaxError::SyntaxError(const std::string& msg, SourceLocation loc)
    : BaseException(msg, loc) {}

std::string SyntaxError::buildMsg() const
{
    std::ostringstream s;
    s << "Line " << loc.y << " : SyntaxError: " << msg;
    return s.str();
}

ReferenceError::ReferenceError(const std::string& msg, SourceLocation loc)
    : BaseException(msg, loc) {}

std::string ReferenceError::buildMsg() const
{
    std::ostringstream s;
    s << "Line " << loc.y << " : ReferenceError: " << msg;
    return s.str();
}
