#include "Table.h"

namespace ql
{
    Table Invalid_Table(true);

    Table& Table::Create_Column(const std::string& name, const std::string& type)
    {
        Columns.emplace(name, Column(type));

        return *this;
    }
}