#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <vector>
#include <string>

class ContextManager
{
    public:
        ContextManager();
        int getStackSize() const;
        std::string peek() const;
        void push(const std::string& fname);
        std::string pop();
        void clear();
    private:
        // stack of file names (how deep are we in include tree)
        std::vector<std::string> incStack;
};

#endif
