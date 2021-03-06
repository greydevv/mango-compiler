#ifndef PATH_H
#define PATH_H

#include <filesystem>

namespace fs = std::filesystem;

struct FilePath
{
    std::string abspath;
    std::string relpath;
    std::string fname;

    FilePath(fs::path original);
    bool exists() const;

    static FilePath asImport(const FilePath& currFp, const std::string& importStr);
};

#endif
