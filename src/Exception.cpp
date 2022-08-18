#include <sstream>
#include <string>
#include "Exception.h"
#include "ContextManager.h"
#include "Token.h"
#include "path.h"
#include "io.h"
#include "Lexer.h"
#include "fmt/color.h"

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
    // TODO: handle underlines that might span multiple lines

    // error should be raised from file at top of stack, i.e. file that is
    // currently being compiled
    const FilePath& fp = ctx.peek();
    std::string errLine = line;
    if (errLine.empty())
    {
        // if line was not supplied, initialize a lexer to retrieve it
        Lexer lexer(fp, ctx);
        errLine = lexer.getLine(loc.y);
    }
    // trim leading and trailing whitespace
    rtrim(errLine);
    int leadingTrimmed = ltrim(errLine); 
    std::string underline;
    
    // create error underline
    if (loc.x-leadingTrimmed-1 < 0 || loc.len-1 < 0)
    {
      // temporary fix for errors without lines
      underline = "";
    } else {
      underline = fmt::format("{}^{}",
          std::string(loc.x-leadingTrimmed-1, ' '),
          std::string(loc.len-1, '~')  // subtract 1 because '^' was inserted
      );
    }

    std::ostringstream s;
    // get blank spaces to span # of literal characters in line #, e.g. 2 if
    // line # is 13
    std::string lineNoSpace = std::string(std::to_string(loc.y).length(), ' ');
    // use relative path for shorter error location (fp.relpath)
    s << fmt::format(fmt::emphasis::bold, fp.relpath);
    s << fmt::format(" -> [{},{}]\n", loc.y, loc.x);
    s << fmt::format("{} |\n", lineNoSpace);
    s << fmt::format("{} | {}\n", loc.y, errLine);
    s << fmt::format("{} | {}\n", lineNoSpace, underline);
    s << fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), "{}", getExcName());
    s << fmt::format(fmt::emphasis::bold, ": {}\n", msg);
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

TypeError::TypeError(const std::string& msg, SourceLocation loc)
    : BaseSourceException(msg, loc) {}

std::string TypeError::getExcName() const {return "TypeError";}
