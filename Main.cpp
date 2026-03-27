#include <optional>
#include "Print.h"
#include "Environment.h"
#include "Match.h"

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
    ql::fmt::Print("\n");
    ql::Environment env;

    ql::Function_Traits<decltype(Add)>::Args t;

    std::tuple<int, int> tup = { 12, 100 };

    ql::Any any_Tuple = tup;
    auto tuple = std::any_cast<ql::Function_Traits<decltype(Add)>::Args>(any_Tuple.Data);

    auto element = std::get<0>(tuple);
    auto element_1 = std::get<1>(tuple);
    ql::fmt::Print("any_Tuple: {}\n", element);
    ql::fmt::Print("any_Tuple: {}\n", element_1);

    test(Add, tuple);
    
    ql::Generic_Function gfn(Add);
    gfn.Function_To_Tuple(Add);
    int ret = gfn(4, 6);
    ql::fmt::Print("ret: {}\n", ret);

    env.Create_Database("Data_0", 
    { 
        { "Table_0", ql::Table({ { "Column_0", ql::Column() } }) } 
    })
    .Create_Table("Table_1", {
        { "Column_0", Tid(int) },
        { "Column_1", Tid(bool) },
        { "Column_2", Tid(float) },
        { "Column_3", Tid(Test) }
    });

    //env.Print_Databases();

    return 0;
}