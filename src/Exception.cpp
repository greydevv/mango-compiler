#include <string>
#include <filesystem>
#include "Exception.h"
#include "Token.h"
#include "fmt/color.h"
#include "io.h"

BaseException::BaseException(const std::string& excName, const std::string& fname, const std::string& msg, SourceLocation loc)
    : excName(excName), fname(fname), msg(msg), loc(loc) {}

std::ostringstream BaseException::getErrMetaStream() const
{
    // builds the metadata that includes the location of the error
    // '[fname]:x:y: [excName]: [msg]'
    std::ostringstream s;
    s << fmt::format(fmt::emphasis::bold, "{}:{}:{}: ", getFileNameFromPath(fname), loc.y, loc.x);
    s << fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), excName);
    s << fmt::format(fmt::emphasis::bold, ": {}\n", msg);
    return s;
}

const char* BaseException::what() const throw()
{
    std::string what = getMsg();
    const char* what_cstr = what.c_str();
    return what_cstr;
}

SyntaxError::SyntaxError(const std::string& fname, const std::string& msg, const std::string& line, SourceLocation loc)
    : BaseException("SyntaxError", fname, msg, loc), line(line) {}

std::string SyntaxError::getMsg() const
{
    std::ostringstream s = getErrMetaStream();
    s << line;
    return s.str();
}

ReferenceError::ReferenceError(const std::string& fname, const std::string& msg, const std::string& line, SourceLocation loc)
    : BaseException("ReferenceError", fname, msg, loc), line(line) {}

std::string ReferenceError::getMsg() const
{
    std::ostringstream s = getErrMetaStream();
    s << line;
    return s.str();
}

NotImplementedError::NotImplementedError(const std::string& fname, const std::string& msg, SourceLocation loc)
    : BaseException("NotImplementedError", fname, msg, loc) {}

std::string NotImplementedError::getMsg() const
{
    return getErrMetaStream().str();
}

std::string getFileNameFromPath(const std::string& fullPath)
{
    return fullPath.substr(fullPath.find_last_of('/')+1);
}

CommandError::CommandError(const std::string& msg)
    : excName("CommandError"), msg(msg) {}

const char* CommandError::what() const throw()
{
    std::ostringstream s;
    s << fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), excName);
    s << fmt::format(fmt::emphasis::bold, ": {}\r", msg);
    std::string what = s.str();
    const char* what_cstr = what.c_str();
    return what_cstr;
}
