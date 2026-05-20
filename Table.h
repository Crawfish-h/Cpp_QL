#pragma once
#include <utility>
#include <vector>
#include <any>
#include <string>
#include "Type.h"
#include "Typedefs.h"
#include "Util.h"

namespace ql
{
    class Table;
    class Database;

    struct Column
    {
        
        std::string Name;
        std::string Type;
        std::vector<Any> Rows;
        Any Metadata;
        Any_Fn Add;
        Any_Fn Sub;
        Any_Fn Mul;
        Any_Fn Div;
        Any_Fn Print;

        Column(const std::string& name, const std::string& type, std::initializer_list<Any> rows, Any metadata, 
            Any_Fn add,
            Any_Fn sub,
            Any_Fn mul,
            Any_Fn div,
            Any_Fn print
        ) : Name(name), Type(type), Rows(rows), Metadata(metadata), Add(add), Sub(sub), Mul(mul), Div(div), Print(print) {}
    };

    class Table
    {
    public:
        Map<std::string, Column> Columns;
        std::string Name;
    };

    extern Table Invalid_Table;
}