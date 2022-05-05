#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <vector>
#include <fstream>
#include "path.h"

class ContextManager
{
    public:
        ContextManager();
        int getStackSize() const;
        const FilePath& peek() const;
        std::shared_ptr<std::ifstream> getIs();
        void push(FilePath fp);
        FilePath pop();
        void open(const FilePath& fp);
        void clear();
    private:
        std::shared_ptr<std::ifstream> is;
        // stack of file names (how deep are we in include tree)
        std::vector<FilePath> incStack;
};

#endif
