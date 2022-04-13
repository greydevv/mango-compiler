#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <map>

template <typename V> 
class SymbolTable
{
    public:
        SymbolTable(const V& defaultLookup);
        bool contains(const std::string& name) const;
        bool insert(const std::string& name, V type);
        V lookup(const std::string& name) const;
        std::pair<bool, std::string>  overwrites(const SymbolTable& other);
        void merge(const SymbolTable& other);
        std::map<std::string, V> getSt() const;
    private:
        std::map<std::string, V> st;
        const V defaultLookup;
};

#endif
