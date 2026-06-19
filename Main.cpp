#include <optional>
#include <print>
#include <vector>
#include <ranges>
#include <cassert>
#include "Print.h"
#include "Match.h"
#include "Typedefs.h"
#include "Enumerable.h"

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

    auto iter = enume.begin();
    iter += 1;
    std::pair<std::string const, int>* pair = *(enume.end() - 1);

    enume_Vec.push_back(Test_Type(2, true));
    Test_Type* tt = *(enume_Vec.begin() + 2 + iter);

    ql::fmt::Print("pair: {}, {}\n", pair->first, pair->second);
    ql::fmt::Print("tt: {}, {}\n", tt->Number, tt->Boolean);
    
    enume_Vec.pop_back();

    for (Test_Type* item: enume_Vec)
    {
        ql::fmt::Print("item: {}, {}\n", item->Number, item->Boolean);
    }

    ql::Variant<int, bool, Test_Type, float> variant = Test_Type(10, true);

    int int_Val = 100;
    bool boolean = false;

    std::optional<ql::Refw<int>> ref_Int = int_Val;
    ql::fmt::Print("int type: {}\n", ql::Tid(int_Val).pretty_name());

    /*ql::Match m(variant);
    (ql::Tid<bool>(), [&]{ bool& val = variant; ql::fmt::Print("bool: {}\n", val); })
    (ql::Tid<Test_Type>(), [&]{ Test_Type& val = variant; ql::fmt::Print("Test_Type: {}, {}\n", val.Number, val.Boolean); })
    (ql::Tid<float>(), [&]{ float& val = variant; ql::fmt::Print("float: {}\n", val); })
    (ql::Tid<int>(), [&]{ int& val = variant; ql::fmt::Print("int: {}\n", val); })
    .Defualt([]{ ql::fmt::Print("Defualt\n"); });*/

    //static_assert(typeid(decltype(map)::key_type) == typeid(int));
    
    return 0;
}