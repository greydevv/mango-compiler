#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include "Types.h"

class SymbolTable
{
    public:
        SymbolTable() {};
        bool contains(const std::string& name);
        bool insert(const std::string& name, Type type);
        Type lookup(const std::string& name);
    private:
        std::map<std::string, Type> st;
};

#endif
