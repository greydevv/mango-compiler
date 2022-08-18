#include <map>
#include "Types.h"
#include "SymbolTable.h"

template <typename V>
SymbolTable<V>::SymbolTable(const V& defaultLookup)
    : st(), defaultLookup(defaultLookup) {}

template <typename V>
bool SymbolTable<V>::contains(const std::string& name) const
{
    return st.contains(name);
}

template <typename V>
bool SymbolTable<V>::insert(const std::string& name, V val)
{
    if (contains(name))
        return false;

    st.insert(std::pair<std::string, V>(name, val));
    return true;
}

template <typename V>
V SymbolTable<V>::lookup(const std::string& name) const
{
    return contains(name) ? st.at(name) : defaultLookup;
}

template <typename V>
std::pair<bool, std::string>  SymbolTable<V>::overwrites(const SymbolTable<V>& other)
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

template <typename V>
void SymbolTable<V>::merge(const SymbolTable<V>& other)
{
    st.merge(other.getSt());
}

template <typename V>
void SymbolTable<V>::clear()
{
    st.clear();
}

template <typename V>
std::map<std::string, V> SymbolTable<V>::getSt() const
{
    return st;
}

template class SymbolTable<Type>;
template class SymbolTable<ProtoSymbol>;
