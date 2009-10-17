#ifndef STACK_H
#define STACK_H

#include "lists.h"
#include <stdlib.h>

namespace ag
{


template<typename T>
class stack : public list<T>
{
    public:
        stack() {};
        ~stack() {};
        listmember<T>* push(T data)
        {
            return add_tail(data);
        };

        T& pop()
        {
            return del(this->tail);
        }
    private:
};


}

#endif // STACK_H
