#include <sstream>
#include <string>
#include "Exception.h"
#include "ContextManager.h"
#include "Token.h"
#include "fmt/color.h"
#include "path.h"
#include "io.h"
#include "Lexer.h"

BaseException::BaseException(const std::string& msg)
    : msg(msg) {}

std::string BaseException::getMsg(ContextManager& ctx) const
{
    std::ostringstream s;
    s << fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), getExcName());
    s << fmt::format(fmt::emphasis::bold, ": {}\n", msg);
    return s.str();
}

const char* BaseException::what() const throw()
{
    // std::string msg = getMsg();
    // const char* what_cstr = msg.c_str();
    // return what_cstr;
    return "Builtin Exception: Use exc.getMsg(ctx) instead\n";
}

BaseSourceException::BaseSourceException(const std::string& msg, const std::string& line, SourceLocation loc)
    : BaseException(msg), line(line), loc(loc) {}

BaseSourceException::BaseSourceException(const std::string& msg, SourceLocation loc)
    : BaseException(msg), line(""), loc(loc) {}

std::string BaseSourceException::getMsg(ContextManager& ctx) const
{
    // use relative path for shorter/readable error message
    std::string tmpLine = line;
    // FilePath object of file that contained the error
    const FilePath& fp = ctx.peek();
    if (line.empty())
    {
        Lexer lexer(fp, ctx);
        tmpLine = underlineError(lexer.getLine(loc.y), loc.x, 1);
    }
    std::ostringstream s;
    s << fmt::format(fmt::emphasis::bold, "{}:{}:{}: ", fp.relpath, loc.y, loc.x);
    s << fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), getExcName());
    s << fmt::format(fmt::emphasis::bold, ": {}\n", msg);
    s << tmpLine << '\n';
    return s.str();
}

CompilationError::CompilationError(const std::string& msg)
    : BaseException(msg) {}

std::string CompilationError::getExcName() const {return "Error";}

NotImplementedError::NotImplementedError(const std::string& msg)
    : BaseException(msg) {}

std::string NotImplementedError::getExcName() const {return "NotImplementedError";}

FileNotFoundError::FileNotFoundError(const FilePath& badFp, const std::string& line, SourceLocation loc)
    : BaseSourceException(badFp.relpath, line, loc) {}

std::string FileNotFoundError::getExcName() const {return "FileNotFoundError";}

SyntaxError::SyntaxError(const std::string& msg, const std::string& line, SourceLocation loc)
    : BaseSourceException(msg, line, loc) {}

std::string SyntaxError::getExcName() const {return "SyntaxError";}

ReferenceError::ReferenceError(const std::string& msg, const std::string& line, SourceLocation loc)
    : BaseSourceException(msg, line, loc) {}

ReferenceError::ReferenceError(const std::string& msg, SourceLocation loc)
    : BaseSourceException(msg, loc) {}

std::string ReferenceError::getExcName() const {return "ReferenceError";}

TypeError::TypeError(const std::string& msg, const std::string& line, SourceLocation loc)
    : BaseSourceException(msg, line, loc) {}

std::string TypeError::getExcName() const {return "TypeError";}
