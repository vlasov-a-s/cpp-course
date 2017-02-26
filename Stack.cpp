//--------------------------------------------------
//! @file Stack.cpp
//! Implements a stack class
//!
//! @author Vlasov Alexander, Feb. 2017
//--------------------------------------------------

#include <cassert>
#include <iostream>
#include <Stack.h>

#define ASSERT_OK() \
    if (!ok())      \
    {               \
        dump();     \
        assert(0);  \
    }               \

using std::cout;
using std::endl;

class Stack;

Stack::Stack(size_t capacity = def_capacity_) :
    size_(0),
    capacity_(capacity)
{
    data_ = new value_type[capacity];
}

Stack::~Stack()
{
    delete[] data_;
    size_ = POISON_VAR;
    capacity_ = 1;
}

size_t Stack::size()
{
    return size_;
}

size_t Stack::capacity()
{
    return capacity_;
}

bool Stack::empty()
{
    return (size() == 0);
}

bool Stack::push(value_type value)
{
    ASSERT_OK();
    if (size()>=capacity())
        return false;
    data_[size_++] = value;
    ASSERT_OK();
    return true;
}

Stack::value_type Stack::top()
{
    ASSERT_OK();
    if (size()!=0)
        return data_[size_-1];
    else
        assert(!"Stack is empty!");
}

Stack::value_type Stack::pop()
{
    ASSERT_OK();
    if (size()!=0)
    {
        value_type p = data_[--size_];
        data_[size_] = POISON_VAR;
        ASSERT_OK();
        return p;
    }
    else
        assert(!"Stack is empty!");
}

bool Stack::ok()
{
    return (size() <= capacity()-1);
}

bool Stack::dump()
{
    cout << "Stack (";
    if (ok())
        cout << "OK)" << endl;
    else
        cout << "ERROR!)" << endl;
    cout << "\t{" << endl;
    cout << "\tsize_\t\t= " << size() << endl;
    cout << "\tcapacity_\t= " << capacity() << endl;
    cout << "\tdata_ [" << capacity() << "]:"<< endl;
    cout << "\t\t{" << endl;
    for (int i=0; i<capacity(); i++)
    {
        if (i<size())
            cout << "\t*\t" << "[" << i << "] = " << data_[i];
        else
            cout << "\t\t" << "[" << i << "] = " << data_[i];
        if (data_[i] == POISON_VAR)
            cout << " (POISON_VAR)" << endl;
        else
            cout << endl;
    }
    cout << "\t\t}" << endl;
    cout << "\t}" << endl;
}
