#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <vector>
#include "path.h"

class ContextManager
{
    public:
        ContextManager();
        int getStackSize() const;
        const FilePath& peek() const;
        void push(FilePath fp);
        FilePath pop();
        void clear();
    private:
        // std::ifstream& is;
        // stack of file names (how deep are we in include tree)
        std::vector<FilePath> incStack;
};

#endif
