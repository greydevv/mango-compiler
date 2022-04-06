#include <string>
#include <filesystem>
#include "Exception.h"
#include "Token.h"
#include "fmt/color.h"
#include "io.h"

BaseException::BaseException(const std::string& msg)
    : msg(msg) {}

std::string BaseException::getFormattedMsg() const
{
    std::ostringstream s;
    s << fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), getExcName());
    s << fmt::format(fmt::emphasis::bold, ": {}\n", msg);
    return s.str();
}

const char* BaseException::what() const throw()
{
    std::string msg = getFormattedMsg();
    const char* what_cstr = msg.c_str();
    return what_cstr;
}

BaseSourceException::BaseSourceException(const std::string& fname, const std::string& msg, const std::string& line, SourceLocation loc)
    : BaseException(msg), fname(fname), line(line), loc(loc) {}

std::string BaseSourceException::getFormattedMsg() const
{
    std::ostringstream s;

    std::string relPath = std::filesystem::relative(fname);
    s << fmt::format(fmt::emphasis::bold, "{}:{}:{}: ", relPath, loc.y, loc.x);
    s << fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), getExcName());
    s << fmt::format(fmt::emphasis::bold, ": {}\n", msg);
    s << line << '\n';
    return s.str();
}

CompilationError::CompilationError(const std::string& msg)
    : BaseException(msg) {}

std::string CompilationError::getExcName() const {return "Error";}

NotImplementedError::NotImplementedError(const std::string& msg)
    : BaseException(msg) {}

std::string NotImplementedError::getExcName() const {return "NotImplementedError";}

FileNotFoundError::FileNotFoundError(const std::string& fname, const std::string& badFname, const std::string& line, SourceLocation loc)
    : BaseSourceException(fname, badFname, line, loc) {}

std::string FileNotFoundError::getExcName() const {return "FileNotFoundError";}

SyntaxError::SyntaxError(const std::string& fname, const std::string& msg, const std::string& line, SourceLocation loc)
    : BaseSourceException(fname, msg, line, loc) {}

std::string SyntaxError::getExcName() const {return "SyntaxError";}

ReferenceError::ReferenceError(const std::string& fname, const std::string& msg, const std::string& line, SourceLocation loc)
    : BaseSourceException(fname, msg, line, loc) {}

std::string ReferenceError::getExcName() const {return "ReferenceError";}
