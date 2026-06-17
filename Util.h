#pragma once
#include <concepts>
#include <type_traits>
#include <iostream>
#include <any>
#include <unordered_map>
#include <string>
#include <functional>
#include <optional>
#include <typeindex>
#include <variant>
#include <initializer_list>
#include <stdexcept>
#include <boost/type_index.hpp>

namespace ql
{       
    #define Tid(T) #T

    // Type id for an expression.
    #define Tide(expr) 

    #define Type_Id(T) \
    boost::typeindex::type_id_with_cvr<decltype(T)>()

    #define Type_Id_T(T) \
    boost::typeindex::type_id_with_cvr<T>()

    using Boost_Typei = boost::typeindex::type_index;

    auto Type_Id_Traits(std::string_view type_Name)
    {
        struct
        {
            bool Is_Ptr = false;
            bool Is_Ref = false;
            bool Is_Const = false;
        } traits;

        size_t str_Pos = type_Name.find("const ");
        if (*std::prev(type_Name.end()) == '&')
        {
            traits.Is_Ref = true;
        }else if (*std::prev(type_Name.end()) == '*')
        {
            traits.Is_Ptr = true;
        }else if (str_Pos != std::string::npos && str_Pos == 0)
        {
            traits.Is_Const = true;
        }

        return traits;
    }

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

        operator std::string()
        {
            return Str;
        }

        //friend 

        std::string Str;
    };

    std::ostream& operator<<(std::ostream& os, const String_Format& str_Format)
    {
        return os << str_Format.Str;
    }

    template<class V, class T>
    bool Holds_Alt(V variant)
    {
        return std::holds_alternative<T>(variant);
    }

    class Poly
    {
    public:
        Poly(){}

        template<class T>
        Poly(T value)
        {
            Pointer = value;
            Type_Index = Type_Id_T(T);
        }

        template<class T>
        operator T() 
        { 
            if (Type_Index != Type_Id_T(T))
            {
                /*throw std::runtime_error(String_Format(
                    "Invalid cast from void* ({}) to {}\n", 
                    Type_Index.pretty_name(), 
                    Type_Id_T(T).pretty_name())
                );*/
            }

            return static_cast<T>(Pointer); 
        }

        const Boost_Typei& Type() const
        {
            return Type_Index;
        }
        
    private:
        Boost_Typei Type_Index = Type_Id(nullptr);
        void* Pointer = nullptr;
    };

    template <typename T>
    inline constexpr bool Is_Not_Pointer_v = not std::is_pointer_v<T>;

    class Any
    {
    public:
        Any(std::any any_Arg) 
        { 
            Data = any_Arg;
            Ref.Data_Ref = &Data; 
            Cast_Fn = nullptr; 
        }

        template<class T>
        Any(T value) 
        { 
            Data = value; 
            Ref.Data_Ref = &Data; 
            Pretty_Type = Type_Id_T(T);

            /*if constexpr (std::is_pointer_v<T>)
            {
                Cast_Fn = [&](std::any any){
                    T val = std::any_cast<T>(any);
                    *val = *std::any_cast<T>(Data);
                };
            }*/
        }

        Any(){ Ref.Data_Ref = &Data; }

        operator std::any() { return Data; }

        template<class T>
        operator T() { return std::any_cast<T>(Data); }

        struct Ref_T
        {
            Ref_T(std::any& data){ Data_Ref = &data; }
            Ref_T(){}
            std::any* Data_Ref = nullptr;

            template<class T>
            operator T&() { return std::any_cast<T>(*Data_Ref); }
        };

        template<class T>
        T& As()
        {
            return std::any_cast<T&>(Data);
        }

        

        template<class T>
        requires Is_Not_Pointer_v<T>
        void Set(T& value)
        {
            if (typeid(value) != Data.type())
            {
                if (Type_Id_Traits(Data.type().name()).Is_Ref)
                {
                    if (Type_Id_Traits(Data.type().name()).Is_Const && not std::is_const_v<T>)
                    {
                        value = As<const T&>();
                        return;
                    }

                    value = As<T&>();
                    return;
                }

                if (Type_Id_Traits(Data.type().name()).Is_Const && not std::is_const_v<T>)
                {
                    value = As<const T>();
                    return;
                }
            }

            value = As<T>();
        }

        template<class T>
        requires requires(T t) { (decltype(t))nullptr; }
        void Set(T value)
        {
            std::cout << "Set ptr\n";
            if (Cast_Fn != nullptr)
            {
                Cast_Fn(value);
                return;
            }
            
            value = As<T>();
        }

        std::function<void(std::any)> Cast_Fn = nullptr;
        std::any Data;
        Ref_T Ref;
        boost::typeindex::type_index Pretty_Type;
    };

    struct Fn_Data
    {
        template<class ...Valid_After_Fn_Data>
        Fn_Data(const std::string& fn_Name, Valid_After_Fn_Data&&... after_Fn_Data)
        : Fn_Name(fn_Name), After_Fn_Data{after_Fn_Data...} {}

        const std::vector<std::string> After_Fn_Data;
        const std::string Fn_Name;
    };


    // Function traits from https://stackoverflow.com/a/57622226, slightly edited.
    template<class Func> 
    class Function_Traits;

    template<class R, class ...FArgs> 
    class Function_Traits<R(FArgs...)>
    {
    public:
        using Args = std::tuple<FArgs...>;
        using Return_T = R;
        using Function_Traits_T = Function_Traits<R(FArgs...)>;
        const size_t Args_Num = std::tuple_size_v<Args>;
        
        const bool Ret_Is_Ptr = std::is_pointer_v<R>;
        const bool Ret_Is_Const = std::is_const_v<R>; 
        const bool Ret_Is_Class = std::is_class_v<R>; 

        // Returns true if Args_Num == 1 and if 
        // typeid(std::get<0>(Args)) == typeid(T);
        template<class T>
        constexpr bool Is_Unary_T()
        {
            //Args tuple = {};
            //return (typeid(std::get<0>(tuple)) == typeid(T)) && Args_Num == 0;
            return false;
        }
    };

    
    #define Fn_Traits(func_Type) typename Function_Traits<std::remove_pointer_t<func_Type>>::Function_Traits_T

    using Any_Fn = Any (*)(Any args);

    struct Generic_Function
    {
        std::function<Any(Any)> GFunction;

        std::type_index Arg_Types;
        std::type_index Return_Type;

        template<class F>
        Generic_Function(F func) : 
            Arg_Types(typeid(Fn_Traits(F)::Args)), 
            Return_Type(typeid(Fn_Traits(F)::Return_T))
        { 
            //using Func_traits = Function_Traits<std::remove_pointer_t<decltype(func)>>;
            
            GFunction = [=](Any args) -> Any
            { 
                auto tuple_Args = std::any_cast<Fn_Traits(F)::Args>(args.Data);

                return std::apply(func, tuple_Args);
            };
        }

        template<class ...Args>
        Any operator()(Args&&... args)
        {
            std::tuple<Args...> tuple_Args(args...);
            return GFunction(tuple_Args);
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

    enum Comp { And, Or };

    template<class T, class ...Args>
    inline bool Eqm(T&& value, Comp comp, Args&&... args)
    {
        if (comp == And)
            return ((value == args) && ...);
        else
            return ((value == args) || ...);
    }

    template <class F, class R, class ...Args>
    concept Callable = std::is_invocable_r_v<R, F, Args...>;

    template <class F>
    concept Callable_F = std::is_invocable<F>::value;

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
        //std::apply
        return 1;
    }
}