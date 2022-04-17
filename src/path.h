#ifndef PATH_H
#define PATH_H

#include <filesystem>

namespace fs = std::filesystem;

struct FilePath
{
    fs::path abspath;
    fs::path relpath;
    fs::path fname;

    FilePath(fs::path original);
};

fs::path getAbsoluteImport(fs::path currentFile, fs::path importFile);

#endif
