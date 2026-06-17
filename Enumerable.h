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
            typedef Poly value_type;
            typedef Poly pointer;
            typedef Poly reference;
            typedef ptrdiff_t difference_type;
            
            Iterator(Enumerable* cont, Poly iter_Ptr, size_t iter_Pos) : 
                Container(cont), Ptr(iter_Ptr), Iter_Pos(iter_Pos) {}

            Iterator(const Iterator& iterator){ *this = iterator; Container = nullptr; }
            ~Iterator(){}

            Iterator& operator++()
            {
                Iter_Pos++;
                return *this;
            }

            Iterator& operator+=(ssize_t rh)
            {
                Iter_Pos += rh;
                return *this;
            }

            Iterator& operator-=(ssize_t rh)
            {
                Iter_Pos -= rh;
                return *this;
            }

            Iterator operator+(ssize_t rh)
            {
                return Iterator(Container, Ptr, Iter_Pos + rh);
            }

            Iterator operator-(ssize_t rh)
            {
                return Iterator(Container, Ptr, Iter_Pos - rh);
            }

            Iterator& operator+=(Iterator& rh)
            {
                Iter_Pos += rh.Iter_Pos;
                return *this;
            }

            Iterator& operator-=(Iterator rh)
            {
                Iter_Pos -= rh.Iter_Pos;
                return *this;
            }

            Iterator operator+(Iterator rh)
            {
                return Iterator(Container, Ptr, Iter_Pos + rh.Iter_Pos);
            }

            Iterator operator-(Iterator rh)
            {
                return Iterator(Container, Ptr, Iter_Pos - rh.Iter_Pos);
            }

            reference operator*()
            {
                return Container->Iter_Deref(Iter_Pos);
            }

            size_t Iter_Pos = 0;
            Poly Ptr;
            Enumerable* Container;
        };

        Enumerable() : Value_Type(typeid(nullptr)) {}

        template<Iterable cont>
        Enumerable(cont&& container) : Value_Type(typeid(nullptr))
        {
            using value_type = typename std::decay_t<cont>::value_type;
            using Vec_Type = std::vector<value_type>;

            Value_Type = typeid(value_type);

            auto& vector = Container.emplace<Vec_Type>();

            for (auto& item : container)
            {
                vector.push_back(item);
            }

            Begin_Fn = [&](){
                return Iterator(this, &*vector.begin(), 0);
            };

            End_Fn = [&](){
                return Iterator(this, &*vector.end(), vector.size() - 1);
            };

            Iter_Deref = [&](size_t iter_Pos) -> Poly {
                return &*(vector.begin() + iter_Pos);
            };
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
        void (*Push_Back)(std::any);
        std::function<Iterator()> Begin_Fn;
        std::function<Iterator()> End_Fn;
        std::function<void(Poly)> Iter_Increment;
        std::function<Poly(size_t)> Iter_Deref;
    };
}