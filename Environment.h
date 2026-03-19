#pragma once
#include <unordered_map>
#include <vector>
#include "Database.h"
#include "Result.h"

namespace ql
{
    class Environment
    {
    public:
        Environment(Init_List_Pair<Database> databases = {},
            Init_List_Pair<Type> types = {}) 
            : Databases(databases.begin(), databases.end()), 
            Types(types.begin(), types.end()) {}

        // Returns false if a database was not created.
        Database& Create_Database(const std::string& name, 
            Init_List_Pair<Table> tables = {}, 
            Init_List_Pair<Type> types = {});

        // Returns false if Databases.size() equals 0.
        bool Print_Databases();

        // If database_Name and table_Name equals "" then prints all tables in Environment.
        // Else, if table_Name equals "" prints all tables from the database Databases[database_Name].
        // Else, prints Table 'table_Name'.
        // If database_Name or table_Name is invalid, then returns a Result with an Error.
        // If no databases or tables exist, returns a Result with a Warning.
        Result_Ty<int> Get(const std::string& database_Name = "", const std::string& table_Name = "", const std::string& column_Name = "");

        std::unordered_map<std::string, Type> Types;
        std::unordered_map<std::string, Database> Databases;
            
        std::vector<std::string> History;

        Result Resulte;

        bool Warn_No_Databases = true;
        bool Warn_Empty_Databases = true;
        bool Warn_Empty_Tables = true;
    };
}