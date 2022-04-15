#include <string>
#include "ContextManager.h"

ContextManager::ContextManager() {}

int ContextManager::getStackSize() const
{
    return incStack.size();
}

std::string ContextManager::peek() const
{
    return incStack[incStack.size()-1];
}

void ContextManager::push(const std::string& fname)
{
    incStack.push_back(fname);
}

std::string ContextManager::pop()
{
    std::string fname = std::move(incStack.back());
    incStack.pop_back();
    return fname;
}

void ContextManager::clear()
{
    incStack.clear();
}
