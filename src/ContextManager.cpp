#include "ContextManager.h"
#include "path.h"

ContextManager::ContextManager() {}

int ContextManager::getStackSize() const
{
    return incStack.size();
}

FilePath ContextManager::peek() const
{
    return incStack[incStack.size()-1];
}

void ContextManager::push(FilePath fp)
{
    incStack.push_back(fp);
}

FilePath ContextManager::pop()
{
    FilePath fp = std::move(incStack.back());
    incStack.pop_back();
    return fp;
}

void ContextManager::clear()
{
    incStack.clear();
}
