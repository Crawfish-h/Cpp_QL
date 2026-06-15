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
        Database& Create_Database(const std::string& database_Name);
        
        Map<std::string, Database> Databases;
    };
}