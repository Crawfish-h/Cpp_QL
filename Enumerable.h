#pragma once
#include <functional>
#include <optional>
#include <stdexcept>
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
            typedef Any reference;
            typedef ptrdiff_t difference_type;
            
            Iterator(Enumerable* cont, size_t iter_Pos) : 
                Enume(cont), Iter_Pos(iter_Pos) {  }

            Iterator(const Iterator& iterator){ *this = iterator; }
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
                return Iterator(Enume, Iter_Pos + rh);
            }

            Iterator operator-(ssize_t rh)
            {
                return Iterator(Enume, Iter_Pos - rh);
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
                return Iterator(Enume, Iter_Pos + rh.Iter_Pos);
            }

            Iterator operator-(Iterator rh)
            {
                return Iterator(Enume, Iter_Pos - rh.Iter_Pos);
            }

            reference operator*()
            {
                return Enume->VTable(&Iter_Pos, Enume->Do_Iter_Deref)
                    .As<Sim_Proxy<Any>>().Value;
            }

            inline bool operator==(const Iterator& rh)
            {
                return Iter_Pos == rh.Iter_Pos && 
                    Enume->Initial_Container_Type == rh.Enume->Initial_Container_Type;
            }

            inline bool operator!=(const Iterator& rh)
            {
                return not (*this == rh);
            }

            size_t Iter_Pos = 0;
            Enumerable* Enume;
        };

        Enumerable() : Value_Type(typeid(nullptr)) {}

        template<Iterable cont>
        Enumerable(cont&& container) : Value_Type(typeid(nullptr))
        {
            using value_type = typename std::decay_t<cont>::value_type;
            using Vec_Type = std::vector<value_type>;

            Initial_Container_Type = Type_Id_T(cont);
            Value_Type = typeid(value_type);

            auto& vector = Container.emplace<Vec_Type>();

            for (auto& item : container)
            {
                vector.push_back(item);
            }

            VTable = [&](Poly poly, std::string_view strv) -> ql::Variant<std::monostate, Sim_Proxy<Any>, Iterator> {
                ql::Variant<std::monostate, Sim_Proxy<Any>, Iterator> variant;
                variant = std::monostate{};
                Sim_Proxy<Any> proxy;
                Any any_Val;

                Match(strv)
                (Do_Begin, [&]{ variant = Iterator(this, 0); })
                (Do_End, [&]{ variant = Iterator(this, vector.size() - 1); })
                (Do_Iter_Deref, [&]{ variant = proxy = any_Val = 
                    &*(vector.begin() + *(poly.As<size_t*>())); })
                (Do_Push, [&]{ vector.push_back(*poly.As<value_type*>()); })
                (Do_Pop, [&]{ vector.pop_back(); })
                .Defualt([&]{ throw std::runtime_error(String_Format("'strv' has an invalid value: {}", strv)); });

                return variant;
            };

            Begin_Fn = [&](){
                return Iterator(this, 0);
            };

            End_Fn = [&](){
                return Iterator(this, vector.size() - 1);
            };

            Iter_Deref = [&](size_t iter_Pos) -> Poly {
                return &*(vector.begin() + iter_Pos);
            };

            Push_B = [&](Poly value) {
                vector.push_back(*value.As<value_type*>());
            };

            Pop_B = [&]()
            {
                vector.pop_back();
            };
        }

        template<Iterable cont, class ...Args>
        Enumerable& Emplace_Cont(Args&&... args)
        {
            Container.emplace(cont(std::forward(args)...));

            return *this;
        }

        template<class T>
        void push_back(T&& value)
        {
            T val = std::forward<T>(value);

            VTable(&val, Do_Push);
            //Push_B(&val);
        }

        void pop_back()
        {
            VTable(nullptr, Do_Pop);
        }

        Iterator begin()
        {
            Iterator& iter = VTable(nullptr, Do_Begin).As<Iterator>();
            //iter.Enume = this;
            return iter;
        }

        Iterator end()
        {
            Iterator& iter = VTable(nullptr, Do_End).As<Iterator>();
            //iter.Enume = this;
            return iter;
        }

        std::any Container;
        Boost_Typei Value_Type;

    private:
        Boost_Typei Initial_Container_Type = typeid(nullptr);
        const std::string Do_Begin = "Do_Begin";
        const std::string Do_End = "Do_End";
        const std::string Do_Push = "Do_Push";
        const std::string Do_Pop = "Do_Pop";
        const std::string Do_Iter_Deref = "Do_Iter_Deref";

        std::function<ql::Variant<std::monostate, Sim_Proxy<Any>, Iterator>(Poly, std::string_view)> VTable;
        std::function<void(Poly)> Push_B;
        std::function<void()> Pop_B;
        std::function<Iterator()> Begin_Fn;
        std::function<Iterator()> End_Fn;
        std::function<Poly(size_t)> Iter_Deref;
    };
}