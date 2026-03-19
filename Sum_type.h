#pragma once
#include <type_traits>
#include <vector>
    
namespace ql
{
    class Sum_Type;

    class Sum_Type_Field_Parent
    {
    public:
        virtual ~Sum_Type_Field_Parent() = default;
        
    protected:
        virtual void Reset_Other() = 0;
        virtual void Reset() = 0;
    };
    
    template<class T, std::vector<Sum_Type_Field_Parent*>* Stf_Vec = nullptr>
    class Sum_Type_Field
    {
        T& Get_Value_()
        {
            if (Has_Value == false)
            {
                Print("ERROR: Tried to get invalid value from option.\n");
                throw();
            }

            return Value;
        }

    public:
        void Reset_Other() override
        {
            if (Stf_Vec != nullptr)
            {
                for (auto stf : Stf_Vec)
                {
                    stf->Reset();
                }
            }
        }

        void Reset() override
        {
            if (Has_Value)
                Value.~T();

            Has_Value = false;
        }

        Sum_Type_Field()
        {
            Has_Value = false;
        }

        Sum_Type_Field(T&& value)
        {
            Value = value;
            Reset_Other();
            Has_Value = true;
        }

        ~Sum_Type_Field()
        {
            if (Has_Value)
                Value.~T();
        }

        operator bool() const
        {
            return Has_Value;
        }

        T& operator=(const T& other)
        {
            if (this == &other)
                return &this;
            
            Value = other;
            Has_Value = true;
            Reset_Other();

            return &this;
        }

        T Or(T&& other_Value)
        {
            return Has_Value ? Value : other_Value;
        }

        template<class F>
        constexpr Sum_Type_Field<std::invoke_result_t<F, T>> Then(F&& f)
        {
            if (Has_Value)
                return F(Value);
            else
                return {};
        }

        template<class F>
        constexpr std::invoke_result_t<F, T> Transform(F&& f)
        {
            if (Has_Value)
                return F(Value);
            else
                return std::invoke_result_t<F, T>{};
        }

        constexpr const T* operator->() const noexcept;
        {
            return &Get_Value_();
        }

        constexpr T* operator->() noexcept;
        {
            return &Get_Value_();
        }

        constexpr const T& operator*() const noexcept;
        {
            return Get_Value_();
        }

        constexpr T& operator*() noexcept;
        {
            return Get_Value_();
        }

        bool Has_Value;
    private:
        union { T Value_; char Empty_; };
    };

    class Sum_Type
    {
    protected:
        struct Sum_Type_Inner_T
        {
            std::vector<Sum_Type_Field_Parent*> PRIVATE_Sum_Type_Field_Fields_;
        } Sum_Type_Inner;
    };

    // Defines a field in a sum type.
    #define Stf(T) Sum_Type_Field<T, &this->Sum_Type_Inner.PRIVATE_Sum_Type_Field_Fields_>
}