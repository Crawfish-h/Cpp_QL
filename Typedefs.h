#pragma once
#include <unordered_map>

namespace ql
{
    template<class Second, class First = std::string>
    using Init_List_Pair = std::initializer_list<std::pair<First, Second>>;

    template<class T>
    using Init_List = std::initializer_list<T>;

    template<class K, class V>
    using Map = std::unordered_map<K, V>;
}