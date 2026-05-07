#pragma once
#include <unordered_map>
#include <tuple>
#include <type_traits>
#include <utility>
#include "Table.h"
#include "Type.h"
#include "Typedefs.h"
#include "Result.h"

namespace ql
{
    class Database
    {
    public:
        Database(){}
        Database(const std::string& name, 
            Init_List_Pair<Table> tables = {}, 
            Init_List_Pair<Type> types = {}) : Name(name), 
            Tables(tables.begin(), tables.end()), 
            Types(types.begin(), types.end()) {}

        Table& Create_Table(std::string name, Init_List_Pair<std::string> columns = {});

        class Return_Base
        {
        public:
            Return_Base(Database& database) : Database_(database) {}

        protected:
            Database& Database_;
        };

        struct From_Return : Return_Base
        {
            Map<std::string_view, Column*> Columns;
        };

        struct Select_Return : Return_Base
        {
            auto From(std::variant<std::string_view, Table*> table)
            {
                From_Return return_Val(Database_);

            }

            std::vector<std::string_view> column_Names;
        };

        template<class ...Args>
        auto Select(Args&&... args)
        {
            Select_Return return_Val(*this);
            return_Val.column_Names = { args... };
            return return_Val;
        }

        // Prints the contents of Database.Select_Data_;
        Database& Print();

        Database& Insert(const std::string& table_Name, Init_List_Pair<std::string> column_Names, Init_List_Pair<To_String> values);

        Result Resultd;

        std::string Name;
        std::unordered_map<std::string, Table> Tables;
        std::unordered_map<std::string, Type> Types;

    private:
        Map<std::string, Column*> Select_Data_;
        Map<std::string, std::any> Return_Data_;
    };
}