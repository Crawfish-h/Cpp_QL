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

        // Only pass strings and callables as arguments to this function.
        // Columns' string names should be before other functions.
        template<class ...Args>
        Database& Select(Args&&... args)
        {
            //std::vector<Generic_Function> functions = { args... };
            
            auto initial_Func = [](){};

            auto call_Arg = [](auto func, auto prev_Func)
            {
                
            };

            return *this;
        }

        std::pair<std::string, Map<std::string*, Column*>> From(const std::string& table_Name, 
            Fn_Data fn_Data = Fn_Data("From", "Where", "Inner", "Left", "Right", "Union"));

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