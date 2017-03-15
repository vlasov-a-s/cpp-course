/*!
    @file Stack.hpp
    @brief Реализация класса Stack
    @author Власов Александр, Татьяна Мамонтова, Алена Бескровная
    @date Март 2017
*/

#include <cassert>
#include <iostream>

#define ASSERT_OK() \
    if (!ok())      \
    {               \
        dump();     \
        throw 1;  \
    }               \

using std::cout;
using std::endl;

template <typename T>
Stack<T>::Stack() :
    data_(def_capacity_),
    size_(0),
    capacity_(def_capacity_)
{
    cout << __PRETTY_FUNCTION__ << endl;
}

template <typename T>
Stack<T>::Stack(size_t capacity) :
    data_(capacity),
    size_(0),
    capacity_(capacity)
{
    cout << __PRETTY_FUNCTION__ << endl;
}

template <typename T>
Stack<T>::~Stack()
{
    cout << __PRETTY_FUNCTION__ << endl;
    size_ = POISON_VAR;
    capacity_ = 0;
}

template <typename T>
bool Stack<T>::empty() const
{
    return (size_ == 0);
}

template <typename T>
bool Stack<T>::push(T value)
{
    ASSERT_OK();
    if (size_ >= capacity_)
        return false;
    data_[size_++] = value;
    return true;
}

template <typename T>
T Stack<T>::top() const
{
    ASSERT_OK();
    if (size_ != 0)
        return data_[size_ - 1];
    else
        throw 2;
}

template <typename T>
bool Stack<T>::pop()
{
    ASSERT_OK();
    if (size_ != 0)
    {
        data_[--size_] = POISON_VAR;
        ASSERT_OK();
        return true;
    }
    return false;
}

template <typename T>
bool Stack<T>::ok() const
{
    return (size_ <= capacity_);
}

template <typename T>
void Stack<T>::dump() const
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
    for (size_t i = 0; i < capacity(); i++)
    {
        if (i < size())
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