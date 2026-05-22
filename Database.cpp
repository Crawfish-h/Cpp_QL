#include "Database.h"
#include <stdexcept>

namespace ql
{
    Table& Database::Create_Table(const std::string& name, Init_List<Column> columns)
    {
        for (auto& i : columns)
        {
            Columns.insert({i.Name, i});
        }
    }
}