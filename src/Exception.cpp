#include <string>
#include <filesystem>
#include "Exception.h"
#include "Token.h"

BaseException::BaseException(const std::string& fname, const std::string& msg, SourceLocation loc)
    : fname(fname), msg(msg), loc(loc) {}

std::ostringstream BaseException::getErrMetaStream() const
{
    std::ostringstream s;
    s << getFileNameFromPath(fname) << ':' << loc.y << ':' << loc.x;
    return s;
}

const char* BaseException::what() const throw()
{
    std::string what = buildMsg();
    const char* what_cstr = what.c_str();
    return what_cstr;
}

SyntaxError::SyntaxError(const std::string& fname, const std::string& msg, SourceLocation loc)
    : BaseException(fname, msg, loc) {}

std::string SyntaxError::buildMsg() const
{
    std::ostringstream s = getErrMetaStream();
    s << " SyntaxError: " << msg;
    return s.str();
}

ReferenceError::ReferenceError(const std::string& fname, const std::string& msg, SourceLocation loc)
    : BaseException(fname, msg, loc) {}

std::string ReferenceError::buildMsg() const
{
    std::ostringstream s = getErrMetaStream();
    s << " SyntaxError: " << msg;
    return s.str();
}

NotImplementedError::NotImplementedError(const std::string& fname, const std::string& msg, SourceLocation loc)
    : BaseException(fname, msg, loc) {}

std::string NotImplementedError::buildMsg() const
{
    std::ostringstream s = getErrMetaStream();
    s << " NotImplementedError: " << msg;
    return s.str();
}

std::string getFileNameFromPath(const std::string& fullPath)
{
    return fullPath.substr(fullPath.find_last_of('/')+1);
}
