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
            typedef const Any& reference;
            typedef ptrdiff_t difference_type;
            
            Iterator(Enumerable& cont, Any iter_Ptr) : Container(&cont) { Ptr = iter_Ptr; }
            Iterator(const Iterator& iterator){ *this = iterator; Container = nullptr; }
            ~Iterator(){}

            Iterator& operator++()
            {
                Container->Iter_Increment(Ptr);
            }

            reference operator*() const
            {
                return Ptr;
            }

            Any Ptr;
            Enumerable* Container;
        };

        template<Iterable cont>
        Enumerable(cont&& container)
        {
            Value_Type = typeid(cont::value_type);

            Insert_Fn = [](std::any arg){
                 
            };

            Iter_Increment = [](Any& any){

            };

            Container = std::forward(container);
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
            std::forward_iterator a;
        }



        std::any Container;
        const std::type_index Value_Type;

    private:
        void (*Insert_Fn)(std::any);
        std::function<Iterator()> Begin_Fn;
        std::function<Iterator()> End_Fn;
        std::function<void(Any&)> Iter_Increment;
    };
}