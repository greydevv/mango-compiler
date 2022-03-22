#include <map>
#include "SymbolTable.h"

bool SymbolTable::contains(const std::string& name)
{
    return st.contains(name);
}

bool SymbolTable::insert(const std::string& name, Type type)
{
    if (!contains(name))
        return false;

    st.insert(std::pair<std::string, Type>(name, type));
    return true;
}

Type SymbolTable::lookup(const std::string& name)
{
    return contains(name) ? st[name] : Type::eNot;
}
