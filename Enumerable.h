#pragma once
#include <typeindex>
#include <optional>
#include <functional>
#include "Util.h"
#include "Typedefs.h"

namespace ql
{
    class Enumerable
    {
    public:
        struct Iterator
        {
            typedef std::forward_iterator_tag iterator_category;
            typedef Any value_type;
            typedef Any* pointer;
            typedef Any reference;
            typedef ptrdiff_t difference_type;
            
            Iterator(Enumerable* cont, Any iter_Ptr) : Container(cont) { Ptr = iter_Ptr; }
            Iterator(const Iterator& iterator){ *this = iterator; Container = nullptr; }
            ~Iterator(){}

            Iterator& operator++()
            {
                Container->Iter_Increment(Ptr);
                return *this;
            }

            reference operator*()
            {
                return Container->Iter_Deref(Ptr);
            }

            Any Ptr;
            Enumerable* Container;
        };

        Enumerable() : Value_Type(typeid(nullptr)) {}

        template<Iterable cont>
        Enumerable(cont&& container) : Value_Type(typeid(nullptr))
        {
            Value_Type = typeid(typename std::decay_t<cont>::value_type);

            

            Container = std::forward<cont>(container);
        }

        template<Iterable cont, class ...Args>
        Enumerable& Emplace_Cont(Args&&... args)
        {
            Container.emplace(cont(std::forward(args)...));

            return *this;
        }

        template<class T>
        void Insert(T value)
        {
            
        }

        Iterator Begin()
        {
            return Begin_Fn();
        }

        std::any Container;
        std::type_index Value_Type;

    private:
        void (*Insert_Fn)(std::any);
        std::function<Iterator()> Begin_Fn;
        std::function<Iterator()> End_Fn;
        std::function<void(Any&)> Iter_Increment;
        std::function<Any(Any&)> Iter_Deref;
    };
}