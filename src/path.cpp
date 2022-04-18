#include "path.h"

// the 'canonical' methods resolve things like '../' and './' in
// the given paths
// for example, a/b/c/../d is resolved to the literal a/b/d
//
// using 'weakly_canonical' does not throw an error if the file is
// not found
// std::filesystem::path absPath = std::filesystem::weakly_canonical(fname);
FilePath::FilePath(fs::path original)
    : abspath(fs::weakly_canonical(original)),
      relpath(fs::relative(abspath, fs::current_path())),
      fname(original.filename()) {}

FilePath FilePath::asImport(const FilePath& currFp, const std::string& importStr)
{
    // get cwd (directory from which the compile command was invoked from)
    fs::path cwd = fs::current_path();
    fs::path currPath = fs::path(currFp.abspath).parent_path() / fs::path(importStr);
    return FilePath(cwd / currPath);
}

bool FilePath::exists() const
{
    return fs::exists(abspath);
}
