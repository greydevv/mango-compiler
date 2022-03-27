#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include "Types.h"

class SymbolTable
{
    public:
        SymbolTable();
        bool contains(const std::string& name) const;
        bool insert(const std::string& name, Type type);
        Type lookup(const std::string& name) const;
        std::pair<bool, std::string>  overwrites(const SymbolTable& other);
        void merge(const SymbolTable& other);
        std::map<std::string, Type> getSt() const;
    private:
        std::map<std::string, Type> st;
};

#endif
