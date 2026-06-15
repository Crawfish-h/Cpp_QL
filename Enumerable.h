#pragma once
#include "Util.h"

namespace ql
{
    class Enumerable
    {
    public:
        Enumerable(){}

        template<Iterable cont>
        Enumerable(cont&& container)
        {

        }

        template<Iterable cont>
        Enumerable& Emplace_Cont()
        {
            
        }

        template<Iterable cont>
        void Emplace()
        {

        }



    private:

    };
}