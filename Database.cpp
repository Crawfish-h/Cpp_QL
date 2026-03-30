#include "Database.h"
#include <stdexcept>

namespace ql
{
    Table& Database::Create_Table(std::string name, Init_List_Pair<std::string> columns)
    {
        // WIP: Add some sort of error state.
        if (not Resultd) return Tables[name];

        Resultd
        .Reset()
        .If(name == "", 
            {"Error: could not create Table '{}' due to invalid name.", name})
        .Elif(Tables.contains(name), 
            {"Error: could not create Table '{}' because the name is in use.", name})
        .Else<Okay>(
            {"Created Table '{}'", name})
        .Print();

        for (auto& [col_Name, type] : columns)
        {
            Tables[name].Columns.emplace(col_Name, Column(type));
        }

        if (not Resultd.Err) 
        {
            Resultd.Data = &Tables[name];
            Tables[name].TDatabase = this;
            for (auto& column : Tables[name].Columns)
            {
                column.second.CTable = &Tables[name];
            }

            return Tables[name];
        }else
        {
            return Tables[name];
        }
    }

    

    /*Database& Database::From_Old(const std::string& table_Name)
    {
        // WIP: Add some sort of error state.
        if (not Resultd) return *this;

        Resultd
        .Reset()
        .If(not Tables.contains(table_Name), 
            {"Error: Database '{}' does not contain a table with name '{}'.", Name, table_Name})
        .Elif<Warn>(Select_Data_.size() == 0, 
            {"Warning: no columns were selected."})
        .Print();

        // WIP: Add some sort of error state.
        if (Resultd.Err || Resultd.Wrn) return *this;

        auto& cols = Tables[table_Name].Columns;
        if (Select_Data_.contains("*"))
        {
            Select_Data_.erase("*");

            for (auto& [name, column] : cols)
            {
                Select_Data_.emplace(name, &column);
            }
        }else
        {
            for (auto& [name, column] : cols)
            {
                Select_Data_[name] = &column;
            }
        }

        return *this;
    }*/

    Database& Database::Print()
    {
        size_t largest_Column_Size = 0;
        for (auto& [name, column] : Select_Data_)
        {
            fmt::Print("[ {} ] ", name);

            if (column->Rows.size() > largest_Column_Size)
            {
                largest_Column_Size = column->Rows.size();
            }
        }

        fmt::Newline();        
        for (size_t i = 0; i < largest_Column_Size; i++)
        {
            for (auto j = Select_Data_.begin(); j != Select_Data_.end(); ++j)
            {
                if (i < j->second->Rows.size())
                {
                    fmt::Print("{}", j->second->Rows[i].As_String());

                    if (j != std::prev(Select_Data_.end()))
                    {
                        fmt::Print(", ");
                    }
                }
            }

            fmt::Newline();
        }

        return *this;
    }

    Database& Database::Insert(const std::string& table_Name, Init_List_Pair<std::string> column_Names, Init_List_Pair<To_String> values)
    {
        return *this;
    }
}