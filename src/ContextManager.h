#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <vector>
#include <fstream>
#include "path.h"

class ContextManager
{
    public:
        ContextManager(const FilePath& fp);
        int getStackSize() const;
        const FilePath& peek() const;
        std::ifstream& getIs();
        void push(FilePath fp);
        FilePath pop();
        void open(const FilePath& fp);
        void clear();
    private:
        std::ifstream is;
        // stack of file names (how deep are we in include tree)
        std::vector<FilePath> incStack;
};

#endif
