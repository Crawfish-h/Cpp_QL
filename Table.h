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

        Column(){}
        
        template<class T>
        Column(const std::string& name, Any metadata = {}, 
            Any_Fn add = nullptr,
            Any_Fn sub = nullptr,
            Any_Fn mul = nullptr,
            Any_Fn div = nullptr,
            Any_Fn print = nullptr,
            Init_List<Any> rows = {}
        ) : Name(name), Metadata(metadata), Add(add), Sub(sub), Mul(mul), Div(div), Print(print), Rows(rows) {}
    };

    template<class T>
    Column Col(const std::string& name, Any metadata = {}, 
        Any_Fn add = nullptr,
        Any_Fn sub = nullptr,
        Any_Fn mul = nullptr,
        Any_Fn div = nullptr,
        Any_Fn print = nullptr,
        Init_List<Any> rows = {}
    ) 
    {
        
    }

    class Table
    {
    public:
        Map<std::string, Column> Columns;
        std::string Name;
    };

    extern Table Invalid_Table;
}