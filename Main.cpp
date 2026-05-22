#include <optional>
#include "Print.h"
#include "Environment.h"
#include "Match.h"
#include <print>

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
    ql::Environment env;
    env.Create_Database("Database_0")
    .Create_Table("Table_0", {Col<int>{"UID"}});
    //.Table_Add_Row(Table, row args...)
    
    return 0;
}