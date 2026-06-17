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
    int num = 101;
    ql::Poly poly = num;
    float* from_Poly = poly;
    ql::fmt::Print("from_Poly: {}\n", *from_Poly);

    ql::Boost_Typei ti = Type_Id_T(int);
    std::type_index tix(typeid(int));
    ql::fmt::Print("ti: {}\n", ti.raw_name());
    ql::fmt::Print("tix: {}\n", tix.name());

    if (ti.raw_name() == tix.name())
        ql::fmt::Print("types are equal\n");

    std::pair<int, std::string> two = { 33, "Pair" };
    auto pair = &two;
    ql::Any any = &two;

    std::pair<int, std::string> val;
    auto duo = &val;
    duo = any;
    //any.Set(duo);
    two.first = 22;
    ql::fmt::Print("duo: {}, {}\n", duo->first, duo->second);

    /*ql::Map<std::string, int> map;
    map.emplace("aa0", 0);
    map.emplace("ab1", 1);
    map.emplace("ac2", 2);
    map.emplace("ad3", 3);

    auto pa = &map.begin();
    std::cout << "map.begin(): " << (**pa).first << std::endl;

    std::vector<Test_Type> vec;
    vec.emplace_back(20, false);
    vec.emplace_back(12, true);
    vec.emplace_back(33, false);
    ql::Enumerable enume(map);
    ql::Enumerable enume_Vec(vec);
    decltype(map)::value_type a;
    decltype(vec)::value_type b;

    //ql::fmt::Print("typeid: {}", typeid(map.begin()).name());
    
    std::cout << "a typeid: " << Type_Id(a).pretty_name() << std::endl;

    ql::Any any = *enume.Begin();
    ql::Any any_Vec = *enume_Vec.Begin();

    std::cout << "any typeid: " << any.Pretty_Type << std::endl;
    //std::pair<std::string, int> pair = any.As<decltype(a)&>();
    std::pair<std::string, int> pair;
    any.Set(pair);

    std::cout << "any_Vec typeid: " << any_Vec.Pretty_Type << std::endl;
    //Test_Type tt = any_Vec.As<decltype(b)&>();
    Test_Type tt;
    any_Vec.Set(tt);

    ql::fmt::Print("pair: {}, {}\n", pair.first, pair.second);
    ql::fmt::Print("tt: {}, {}\n", tt.Number, tt.Boolean);

    //static_assert(typeid(decltype(map)::key_type) == typeid(int));*/
    
    return 0;
}