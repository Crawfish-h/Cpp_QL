#include <optional>
#include <print>
#include <vector>
#include <ranges>
#include <cassert>
#include "Print.h"
#include "Match.h"
#include "Typedefs.h"
#include "Enumerable.h"


#define Lambda(block) [&](auto arg) -> void* { block return nullptr; }

struct Test_Type
{
    Test_Type(int number, bool boolean) : Number(number), Boolean(boolean) {}
    Test_Type(){}
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

    //auto pa = &*map.begin() + 2;
    //ql::fmt::Print("map.begin(): {}, {}\n", pa->first, pa->second);

    std::vector<Test_Type> vec;
    vec.emplace_back(20, false);
    vec.emplace_back(12, true);
    vec.emplace_back(33, false);

    size_t index = 1;
    auto v = &*vec.end() - index;
    //ql::fmt::Print("vec.begin(): {}, {}\n", v->Number, v->Boolean);

    ql::Enumerable enume(map);
    ql::Enumerable enume_Vec(vec);
    decltype(map)::value_type a;
    decltype(vec)::value_type b;

    //ql::fmt::Print("typeid: {}", typeid(map.begin()).name());
    
    std::cout << "a typeid: " << Type_Id(a).pretty_name() << std::endl;

    auto iter = enume.Begin();
    iter += 1;
    std::pair<std::string const, int>* pair= *(enume.Begin() + 1);
    Test_Type* tt = *(enume_Vec.Begin() + 1 + iter);

    ql::fmt::Print("pair: {}, {}\n", pair->first, pair->second);
    ql::fmt::Print("tt: {}, {}\n", tt->Number, tt->Boolean);

    //static_assert(typeid(decltype(map)::key_type) == typeid(int));
    
    return 0;
}