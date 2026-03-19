#pragma once
#include <unordered_map>
#include <typeindex>
#include <boost/type_index.hpp>
#include <string>
#include "Typedefs.h"

/*
    Using solely typeinfo to manage type information would be ideal, but 
    typeinfo.name being implementation defined means that only using it
    is not a good idea because two different c++ implementations' 
    typeinfo.name values might be incompatible with each other.
*/

//#define Tid(T) Set_Primitive_Type_Name( \
//    boost::typeindex::type_id_with_cvr<T>())

// Type id.
#define Tid(T) #T

// Type id for an expression.
#define Tide(expr) 

#define Type_Id(T) \
    boost::typeindex::type_id_with_cvr<T>().pretty_name()

namespace ql
{
    using Type_Index = boost::typeindex::type_index;

    class Type
    {
    public:
        Type(){}

        Type(const std::string& name, Init_List_Pair<const std::string&> fields)
        {
            Name = name;
            Fields.insert(fields.begin(), fields.end());
        }

        Type(const std::string& name, const std::string& field)
        {
            Name = name;
            Fields.insert({name, field});
        }

        std::string Name;
        std::unordered_map<std::string, std::string> Fields;
    };

    Type_Index&& Set_Primitive_Type_Name(Type_Index&& type);
}