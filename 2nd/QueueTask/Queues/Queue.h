#pragma once

template <class T>
class IQueue
{
public:
    virtual void push(T val) = 0;
    virtual bool pop(T& val) = 0;
};
