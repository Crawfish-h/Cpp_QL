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

auto Test(std::initializer_list<int> ints, const size_t size)
{
    std::vector<int> vec = ints;

    return ql::To_Tuple<size>(vec);
}

int main()
{
    ql::fmt::Print("\n");
    ql::Environment env;

    std::vector<int> vec = { 2, 4, 6, 8, 10, 12 };

    auto tuple = ql::To_Tuple<6>(vec);
    ql::fmt::Print("Tuple: {}\n", std::get<0>(tuple));
    ql::fmt::Print("Tuple: {}\n", std::get<1>(tuple));
    ql::fmt::Print("Tuple: {}\n", std::get<2>(tuple));
    ql::fmt::Print("Tuple: {}\n", std::get<3>(tuple));
    ql::fmt::Print("Tuple: {}\n", std::get<4>(tuple));
    ql::fmt::Print("Tuple: {}\n", std::get<5>(tuple));

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