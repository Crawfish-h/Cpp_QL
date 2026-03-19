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

int main()
{
    ql::fmt::Print("\n");
    ql::Environment env;

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