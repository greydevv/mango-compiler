#ifndef PATH_H
#define PATH_H

#include <filesystem>

namespace fs = std::filesystem;

fs::path getAbsoluteImport(fs::path currentFile, fs::path importFile);

#endif
