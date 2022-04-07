#include <filesystem>
#include "path.h"

fs::path getAbsoluteImport(fs::path currentFile, fs::path importFile)
{
    // Creates an absolute import path based off of currentFile
    // This is meant to create absolute paths from relative import definitions
    fs::path cwd = fs::current_path();
    fs::path currPath = currentFile.parent_path() / importFile;
    return cwd / currPath;
}
