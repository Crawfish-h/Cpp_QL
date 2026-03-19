#pragma once
#include <any>
#include <string>
#include <optional>
#include <variant>
#include "Util.h"
#include "Print.h"

namespace ql
{
    enum class eErr { Value, Creation, Generic, Bounds };
    enum class eOk { Value, Creation, Generic };

    class Reason
    {
    public:
        Reason(const String_Format& message) { Message = message.Str; }
        Reason(){}

        Reason& operator=(const std::string& other)
        {
            Message = other;
            return *this;
        }

        Reason& operator=(const String_Format& other)
        {
            Message = other.Str;
            return *this;
        }

        std::string Message;
        std::any Data;
    };

    // The Reason Okay class. Signifies success.
    class Okay : public Reason
    {
    public:
        Okay(const String_Format& message, const std::any& data = {}, eOk ok_Group = eOk::Generic)
        {
            Ok_Group = ok_Group;
            Message = message.Str; 
            Data = data;
        }

        eOk Ok_Group = eOk::Generic;
    };

    // The Reason Warn class. Signifies a minor problem.
    class Warn : public Reason
    {
    public:
        Warn(const String_Format& message, const std::any& data, eErr wrn_Group = eErr::Generic)
        {
            Wrn_Group = wrn_Group;
            Message = message.Str; 
            Data = data;
        }

        eErr Wrn_Group = eErr::Generic;
    };

    // The Reason Error class. Signifies something pretty bad, but allows for the program to function with major issues.
    class Error : public Reason
    {
    public:
        Error(const String_Format& message, const std::any& data, eErr err_Group = eErr::Generic)
        {
            Err_Group = err_Group;
            Message = message.Str; 
            Data = data;
        }

        eErr Err_Group = eErr::Generic;
    };

    // The Reason Fatal class. Signifies a major problem that will cause the prorgam to be unusable.
    class Fatal : public Error
    {
        using Error::Error;
    };
    

    /*
        Match(v)
        {
            const std::pair<double, double>& => print(v)
        }
    */
    
    class Result : public Eq, public Cast
    {
    public:
        std::optional<Okay> Ok;
        std::optional<Warn> Wrn;
        std::optional<Error> Err;
        std::optional<Fatal> Ftl;

        Result(){}
        Result(const std::any& data, 
            const std::optional<Okay>& ok, const std::optional<Warn>& wrn,
            const std::optional<Error>& err, const std::optional<Fatal>& ftl)
        {
            Data = data;
            Ok = ok;
            Wrn = wrn;
            Err = err;
            Ftl = ftl;
        }

        Result& Reset()
        {
            Data.reset();
            Ok.reset();
            Wrn.reset();
            Err.reset();
            Ftl.reset();

            return *this;
        }

        // Returns true if Err and Ftl equal false.
        operator bool() const
        {
            if (not Err && not Ftl)
            {
                return true;
            }

            return false;
        }

        // Make an Okay object and put it inside the Result.Ok optional.
        Result& mOk(const String_Format& message, const std::any& data = {}, eOk ok_Group = eOk::Generic)
        {
            Ok = Okay(message, data, ok_Group);
            return *this;
        }

        // Make a Warn object and put it inside the Result.Wrn optional.
        Result& mWrn(const String_Format& message, const std::any& data = {}, eErr err_Group = eErr::Generic)
        {
            Wrn = Warn(message, data, err_Group);
            return *this;
        }

        // Make an Error object and put it inside the Result.Err optional.
        Result& mErr(const String_Format& message, const std::any& data = {}, eErr err_Group = eErr::Generic)
        {
            Err = Error(message, data, err_Group);
            return *this;
        }

        // Make a Fatal object and put it inside the Result.Ftl optional.
        Result& mFtl(const String_Format& message, const std::any& data = {}, eErr err_Group = eErr::Generic)
        {
            Ftl = Fatal(message, data, err_Group);
            return *this;
        }

        template<class R = Error>
        Result& If(bool expr, const String_Format& message, const std::any& data = {}, eErr err_Group = eErr::Generic, eOk ok_Group = eOk::Generic)
        {
            Expr_Is_True_ = expr;
            if (Expr_Is_True_ == true)
            {
                Create_Reason_<R>(message, data, err_Group, ok_Group);
            }

            return *this;
        }

        template<class R = Error>
        Result& Else(const String_Format& message, const std::any& data = {}, eErr err_Group = eErr::Generic, eOk ok_Group = eOk::Generic)
        {
            if (Expr_Is_True_ == false)
            {
                Create_Reason_<R>(message, data, err_Group, ok_Group);
            }

            return *this;
        }

        template<class R = Error>
        Result& Elif(bool expr, const String_Format& message, const std::any& data = {}, eErr err_Group = eErr::Generic, eOk ok_Group = eOk::Generic)
        {
            if (Expr_Is_True_ == false)
            {
                Expr_Is_True_ = expr;
                if (Expr_Is_True_ == true)
                {                    
                    Create_Reason_<R>(message, data, err_Group, ok_Group);
                }
            }

            return *this;
        }

        Result& Print()
        {
            if (Ok) ql::fmt::Print("{}\n", Ok->Message);
            if (Wrn) ql::fmt::Print("{}\n", Wrn->Message);
            if (Err) ql::fmt::Print("{}\n", Err->Message);
            if (Ftl) ql::fmt::Print("{}\n", Ftl->Message);

            return *this;
        }

        // This function only calls block() if both Err.has_value and Ftl.has_value equals false.
        // Runs Result.Printr() and calls the lambda/block arg.
        template<class B>
        Result& FElse(B block)
        {
            if (not Err && not Ftl)
            {
                Print();
                block();
            }

            return *this;
        }

    private:
        bool Used_ = false;
        bool Expr_Is_True_ = false;

        template<class R = Error>
        void Create_Reason_(const String_Format& message, const std::any& data = {}, eErr err_Group = eErr::Generic, eOk ok_Group = eOk::Generic)
        {
            if (std::is_same_v<R, Error>)
            {
                Err = Error(message, data, err_Group);
            }else if (std::is_same_v<R, Warn>)
            {
                Wrn = Warn(message, data, err_Group);
            }else if (std::is_same_v<R, Fatal>)
            {
                Ftl = Fatal(message, data, err_Group);
            }else
            {
                Ok = Okay(message, data, ok_Group);
            }
        }
    };

    template<class T>
    class Result_Ty : public Result
    {
    public:
        T* Value;

        T* operator->()
        {
            return Value;
        }
    };
}