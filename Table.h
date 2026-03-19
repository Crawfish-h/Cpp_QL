#pragma once
#include <unordered_map>
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
        Column(){}
        Column(const std::string& type)
            : CType(type) {}

        std::string CType;

        // Originally, this was a vector of std::any, However, using std::any prevented 
        // the ability for users to easily print their own types without having to edit
        // Some sort of potentially large switch statement. 
        // So, using a rust-like trait system allows for better ergonomics.
        std::vector<To_String> Rows;
        Table* CTable; // The Table that owns this Column.
    };

    class Table
    {
    public:
        Table(Init_List_Pair<Column> columns = {})
            : Columns(columns.begin(), columns.end()) {}

        Table(bool is_Invalid) { Is_Invalid = is_Invalid; }

        Table& Create_Column(const std::string& name, const std::string& type);

        // The string key equals the name of the column value.
        std::unordered_map<std::string, Column> Columns;

        Database* TDatabase; // The Database that owns this table.
        bool Is_Invalid = false;
    };

    extern Table Invalid_Table;
}