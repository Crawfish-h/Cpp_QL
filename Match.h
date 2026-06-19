#pragma once
#include <tuple>
#include <any>
#include "Util.h"

namespace ql
{
    struct Match_Break_Type {};
    extern Match_Break_Type Match_Break;

    template<class ...Args>
    class Match
    {
        struct Defualt_Return
        {
            Defualt_Return(const Any& values, 
                const std::tuple<Args...>& match_Args)
            {
                Values = values;
                Match_Args = match_Args;
            }

            Any Values;
            std::tuple<Args...> Match_Args;
        };

    public:
        Match(Args... values)
        {
            Values_ = { values... };
            Return_Value_ = {};
            Return_Valid_Match_Arg_ = {};
        }

        template<class F>
        Match<Args...>& operator()(Args... values, F block)
        {
            if (Match_Broken_ == false)
            {
                std::tuple<Args...> case_Values(values...);
                
                if (case_Values == Values_)
                {
                    Found_Value_ = true;

                    if constexpr (not std::is_same_v<ql::Return_Type_T<decltype(block)>, void>)
                    {
                        Return_Value_ = Call_Fn_With_Args(block, Values_);
                    }else
                    {
                        Call_Fn_With_Args(block, Values_);
                    }
                    
                    Return_Valid_Match_Arg_ = case_Values;

                    if (Return_Value_.Pretty_Type == typeid(Match_Break_Type))
                    {
                        Match_Broken_ = true;
                    }
                }
            }

            return *this;
        }

        template<class F>
        Defualt_Return Defualt(F block_No_Args)
        {
            if (Found_Value_ == false)
            {
                if constexpr (not std::is_same_v<ql::Return_Type_T<decltype(block_No_Args)>, void>)
                {
                    Return_Value_ = block_No_Args();
                }else
                {
                    block_No_Args();
                }
            }

            return Defualt_Return(Return_Value_, Return_Valid_Match_Arg_);
        }

    private:
        bool Found_Value_ = false;
        bool Match_Broken_ = false;
        std::tuple<Args...> Values_;
        std::tuple<Args...> Return_Valid_Match_Arg_;
        Any Return_Value_;
    };
}