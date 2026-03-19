#pragma once
#include <vector>
#include <any>
#include <string>
#include <exception>
#include "Typedefs.h"
#include "Util.h"

namespace ql
{
    enum class Error_Group { Value, Creation, Generic, Bounds };
    enum class Okay_Group { Value, Creation, Generic };

    class Reason
    {
    public:
        std::string Message;
        std::any Data;
    };

    class Okay : public Reason
    {
    public:
        Okay(const std::string& message, const std::any& data, Okay_Group ok_Group = Okay_Group::Generic)
        {
            Ok_Group = ok_Group;
            Message = message; 
            Data = data;
        }

        Okay_Group Ok_Group;
    };

    class Error : public Reason
    {
    public:
        Error(const std::string& message, const std::any& data, Error_Group err_Group = Error_Group::Generic)
        {
            Err_Group = err_Group;
            Message = message; 
            Data = data;
        }

        Error_Group Err_Group;
    };

    class Fatal : public Error
    {};

    class Result
    {
    public:
        Result(){}
        ~Result()
        {
            if (Used == false)
                throw std::runtime_error("ERROR: Didn't use result.\n");
        }

        template<Container oks_T, Container errs_T, Container fatals_T>
        Result(const oks_T& oks, const errs_T& errs, const fatals_T& ftls) : 
            Oks(oks.begin(), oks.end()), 
            Errs(errs.begin(), errs.end()), 
            Ftls(ftls.begin(), ftls.end()) {}
        

        Result operator+(const Result& other)
        {
            std::vector<Okay> oks;
            std::vector<Error> errs;
            std::vector<Fatal> ftls;

            auto insert = [](auto& vec, auto& vec_Other)
            {
                vec.insert(vec.end(), vec_Other.begin(), vec_Other.end());
            };

            insert(oks, Oks);
            insert(oks, other.Oks);
            insert(errs, Errs);
            insert(errs, other.Errs);
            insert(ftls, Ftls);
            insert(ftls, other.Ftls);
            
            return Result(oks, errs, ftls);
        }

        Result& operator+=(const Result& other)
        {
            Oks.insert(Oks.end(), other.Oks.begin(), other.Oks.end());
            Errs.insert(Errs.end(), other.Errs.begin(), other.Errs.end());
            Ftls.insert(Ftls.end(), other.Ftls.begin(), other.Ftls.end());

            return *this;
        }

        // Emplaces an Ok reason into this Result's Oks vector.
        Result& Ok(const std::string& message, const std::any& data = {}, Okay_Group ok_Group = Okay_Group::Generic)
        {
            Oks.emplace_back(message, data, ok_Group);
            return *this;
        }

        // Emplaces an Error reason into this Result's Errs vector.
        Result& Err(const std::string& message, const std::any& data = {}, Error_Group err_Group = Error_Group::Generic)
        {
            Errs.emplace_back(message, data, err_Group);
            return *this;
        }

        // Emplaces a Fatal reason into this Result's Ftls vector.
        Result& Ftl(const std::string& message, const std::any& data = {}, Error_Group err_Group = Error_Group::Generic)
        {
            Ftls.emplace_back(message, data, err_Group);
            return *this;
        }

        Result& If(bool expr, const std::string& message, const std::any& data = {}, Okay_Group ok_Group = Okay_Group::Generic)
        {
            Expr_Is_True = expr;
            if (Expr_Is_True)
                Oks.emplace_back(message, data, ok_Group);

            return *this;
        }

        template<class T = Error>
        Result& Else(const std::string& message, const std::any& data = {}, Error_Group err_Group = Error_Group::Generic)
        {
            if (Expr_Is_True == false)
            {
                if (std::is_same_v<T, Error>)
                {
                    Errs.emplace_back(message, data, err_Group);
                }else
                {
                    Ftls.emplace_back(message, data, err_Group);
                }
            }

            return *this;
        }

        std::any Data;
        std::vector<Okay> Oks;
        std::vector<Error> Errs;
        std::vector<Fatal> Ftls;

    private:
        bool Used = false;
        bool Expr_Is_True = false;
    };
}