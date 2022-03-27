#include <map>
#include "SymbolTable.h"

SymbolTable::SymbolTable()
    : st() {}

bool SymbolTable::contains(const std::string& name) const
{
    return st.contains(name);
}

bool SymbolTable::insert(const std::string& name, Type type)
{
    if (contains(name))
        return false;

    st.insert(std::pair<std::string, Type>(name, type));
    return true;
}

Type SymbolTable::lookup(const std::string& name) const
{
    return contains(name) ? st.at(name) : Type::eNot;
}

std::pair<bool, std::string>  SymbolTable::overwrites(const SymbolTable& other)
{
    // returns <true, 'id'> if merging the two maps would create duplicate keys
    std::pair<bool, std::string> conflict(false, "");
    for (auto const& [id, type] : st)
    {
        if (other.contains(id))
        {
            conflict.first=true;
            conflict.second=id;
            break;
        }
    }
    return conflict;
}

void SymbolTable::merge(const SymbolTable& other)
{
    st.merge(other.getSt());
}

std::map<std::string, Type> SymbolTable::getSt() const
{
    return st;
}
