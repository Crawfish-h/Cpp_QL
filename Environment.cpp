#include "Environment.h"

namespace ql
{
    Database& Environment::Create_Database(const std::string& name, 
        Init_List_Pair<Table> tables, 
        Init_List_Pair<Type> types)
    {
        Resulte
        .Reset()
        .If(name == "", 
            {"Error: could not create database '{}' due to invalid name.", name})
        .Elif(Databases.contains(name), 
            {"Error: could not create database '{}' because the name is in use.", name})
        .Elif(not Databases.emplace(name, Database(name, tables, types)).second, 
            {"Error: could not create database '{}' because function 'Databases.emplace(...)' failed.", name})
        .Else<Okay>(
            {"Created database '{}'", name})
        .Print();

        if (not Resulte.Err) 
        {
            for (auto& table : Databases[name].Tables)
            {
                table.second.TDatabase = &Databases[name];
                for (auto& column : table.second.Columns)
                {
                    column.second.CTable = &table.second;
                }
            }
        }

        return Databases[name];
    }

    bool Environment::Print_Databases()
    {
        if (Databases.size() == 0)
            return false;

        fmt::Print("Databases: \n");
        for (const auto& database : Databases)
        {
            fmt::Print("| {}\n", database.first);
        }

        fmt::Print("\n");
            
        return true;
    }

    Result_Ty<int> Environment::Get(const std::string& database_Name, const std::string& table_Name, const std::string& column_Name)
    {        
        Result_Ty<int> result;

        auto check = [&](bool expr, const std::string& msg)
        {
            result.If(expr,  {msg, database_Name});

        };

        result
        .If(not Databases.contains(database_Name), 
            {"Error: Database with name '{}' does not exist.", database_Name})
        .Elif(not Databases[database_Name].Tables.contains(table_Name) && table_Name != "", 
            {"Error: Table with name '{}' does not exist.", table_Name})
        .Elif(not Databases[database_Name].Tables[table_Name].Columns.contains(table_Name) && column_Name != "", 
            {"Error: Column with name '{}' does not exist.", table_Name})
        .Elif<Warn>(Databases.size() == 0 && Warn_No_Databases, 
            "Warning: Cannot print databases because there are none.")
        .FElse([]
        {
            
        });
            
        return result;
    }
}