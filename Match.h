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
            Defualt_Return(const std::vector<std::any>& values, 
                const std::vector<std::tuple<Args...>>& match_Args)
            {
                Values = values;
                Match_Args = match_Args;
            }

            std::vector<std::any> Values;
            std::vector<std::tuple<Args...>> Match_Args;
        };

    public:
        Match(Args... values)
        {
            Values_ = { values... };
            Return_Values_ = {};
            Return_Valid_Match_Args_ = {};
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
                    Return_Values_.push_back(Func_Rem_Void(block));
                    Return_Valid_Match_Args_.push_back(case_Values);

                    if (Return_Values_.back().type() == typeid(Match_Break_Type))
                    {
                        Match_Broken_ = true;
                    }
                }
            }

            return *this;
        }

        template<class F>
        Defualt_Return Defualt(F block)
        {
            if (Found_Value_ == false)
            {
                Return_Values_.push_back(Func_Rem_Void(block));
                if (Return_Values_.back().type() == typeid(nullptr))
                {
                    Return_Values_.pop_back();
                }
            }

            return Defualt_Return(Return_Values_, Return_Valid_Match_Args_);
        }

    private:
        bool Found_Value_ = false;
        bool Match_Broken_ = false;
        std::tuple<Args...> Values_;
        std::vector<std::any> Return_Values_;
        std::vector<std::tuple<Args...>> Return_Valid_Match_Args_;
    };
}