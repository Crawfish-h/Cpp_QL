#include "Environment.h"

namespace ql
{
    Database& Environment::Create_Database(const std::string& database_Name)
    {
        Databases.emplace(database_Name);
        return Databases[database_Name];
    }
}