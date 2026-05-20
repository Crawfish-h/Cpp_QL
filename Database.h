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
        Database(const std::string& name) : Name(name) {}

        Table& Create_Table(const std::string& name);

    private:
        std::string Name;
    };
}