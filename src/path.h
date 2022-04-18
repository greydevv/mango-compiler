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

    static FilePath asImport(FilePath currFp, const std::string& importStr);
};

#endif
