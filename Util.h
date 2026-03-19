#pragma once
#include <concepts>
#include <type_traits>
#include <iostream>
#include <any>
#include <unordered_map>
#include <string>
#include <functional>
#include <optional>

namespace ql
{
        //#define Fptr(ret, name, args) ret (*name)args

        struct Fn_Data
        {
            template<class ...Valid_After_Fn_Data>
            Fn_Data(const std::string& fn_Name, Valid_After_Fn_Data&&... after_Fn_Data)
            : Fn_Name(fn_Name), After_Fn_Data{after_Fn_Data...} {}

            const std::vector<std::string> After_Fn_Data;
            const std::string Fn_Name;
        };

        using Generic_Fptr = std::any (*)(std::initializer_list<std::any>);

        struct Generic_Function
        {
            std::function<std::any(std::initializer_list<std::any>)> GFunction;

            template<class F>
            Generic_Function(F func) 
            { 
                GFunction = [&](std::initializer_list<std::any> args) -> std::any 
                { 
                    return func();
                }; 
            }

            template<class ...Args>
            std::any operator()(Args&&... args)
            {
                return GFunction(args...);
            }
        };

        struct Filler_T__ {};
        void Impl_Trait(auto& trait, Filler_T__ value) {}

        struct To_String
        {
            To_String(auto&& value)
            {
                Impl_Trait(*this, value);
                Data = value;
            }

            std::any Data;

            std::function<std::string()> As_String;
        };

        template <class T> 
        requires requires(T val) { std::to_string(val); }
        void Impl_Trait(To_String& trait, T value)
        {
            trait.As_String = [&](){ return std::to_string(std::any_cast<decltype(value)>(trait.Data)); };
        }

    class Eq
    {
    public:
        bool operator== (const Eq& eq) const = default;
    };

    class Cast
    {
    public:
        std::any Data;

        template<class T, class U>
        U& as(T& var, U&& type_Var)
        {
            return static_cast<U&>(var);
        }

        template<class U>
        U& as(std::any& var, U&& type_Var)
        {
            return *std::any_cast<U*>(var);
        }

        template<class U>
        U& as(U&& type_Var)
        {
            return *std::any_cast<U*>(Data);
        }

        template<class U>
        U& as()
        {
            return *std::any_cast<U*>(Data);
        }
    };

    struct String_Format
    {
        String_Format(){};

        template<class ...Args>
        String_Format(std::string_view str, Args&&... args)
        {
            Str = std::vformat(str, std::make_format_args(args...));
        }

        String_Format(std::string_view str)
        {
            Str = str;
        }

        String_Format(const std::string&& str)
        {
            Str = str;
        }

        String_Format(const char* ch)
        {
            Str = ch;
        }

        std::string Str;
    };

    enum Comp { And, Or };

    template<class T, class ...Args>
    inline bool Eqm(T&& value, Comp comp, Args&&... args)
    {
        if (comp == And)
            return ((value == args) && ...);
        else
            return ((value == args) || ...);
    }

    template <class F>
    concept Callable = std::is_invocable<F>::value;

    template <class F, class R, class ...Args>
    concept Callable_Ts = std::is_invocable_r_v<R, F, Args...>;

    template<class F>
    concept Returns_Void = requires(F f)
    {
        { f() } -> std::convertible_to<void>;
    };

    template<Returns_Void F>
    void* Func_Rem_Void(F block)
    {
        block();
        return nullptr;
    }

    template<class F>
    auto Func_Rem_Void(F block)
    {
        return block();
    }

    template<class T>
    concept Iterable = requires(T t)
    {
        requires (!std::is_same<T, std::string>::value);
        t.begin();
        t.end();
    } && !std::same_as<std::remove_cvref_t<T>, std::string>;

    template<class T>
    concept Container = requires(T t)
    {
        requires (!std::is_same<T, std::string>::value);
        t.begin();
        t.end();
        t.size();
    } && !std::same_as<std::remove_cvref_t<T>, std::string>;

    template<Container C>
    auto Get(C&& container, size_t stop_Index)
    {
        size_t current_Index = 0;
        for (auto& element : container)
        {
            if (current_Index == stop_Index)
            {
                return element;
            }

            current_Index++;
        }

        return typename std::decay_t<C>::value_type{};
    }

    template<class C, size_t... S_Args>
    auto To_Tuple_Helper(C&& container, std::index_sequence<S_Args...> seq)
    {
        auto element_To_Tuple = [](C&& con, size_t index){ return Get(con, index); }; 
        return std::make_tuple(element_To_Tuple(container, S_Args)...);
    }

    template<size_t N, class C>
    auto To_Tuple(C&& container)
    {
        return To_Tuple_Helper(container, std::make_index_sequence<N>{});
    }

    template<class Func, Container Con_T>
    auto Apply(Func&& func, Con_T&& container)
    {
        for (auto& element : container)
        {
            //std::invoke()
        }
        std::apply
        return 1;
    }
}