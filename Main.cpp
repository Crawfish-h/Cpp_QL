#include <optional>
#include <print>
#include <vector>
#include <ranges>
#include "Print.h"
#include "Match.h"
#include "Typedefs.h"
#include "Enumerable.h"


#define Lambda(block) [&](auto arg) -> void* { block return nullptr; }

struct Test_Type
{
    int Number = 100;
    bool Boolean = true;
};

int Add(int a, int b)
{
    ql::fmt::Print("Add: {}\n", a + b);
    return a + b;
}

template<class F, class Tuple>
void test(F func, Tuple tuple)
{
    std::apply(func, tuple);
}

int main()
{
    ql::Map<std::string, int> map;
    map.emplace("aa0", 0);
    map.emplace("ab1", 1);
    map.emplace("ac2", 2);
    map.emplace("ad3", 3);
    
    std::vector<int> vecint;
    vecint.begin();
    map.begin();

    //static_assert(typeid(decltype(map)::key_type) == typeid(int));
    
    return 0;
}