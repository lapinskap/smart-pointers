#pragma once
#include "Object.h"

// A shared pointer that keeps track on the reference count
// Requires the object to contain the ref counter
// That limits its usage but makes it the safest option

struct IntrusivePtr
{
    IntrusivePtr(Object* obj)
    {
        ptr = obj;
        AddRef();
    }

    IntrusivePtr(IntrusivePtr& other)
    {
        ptr = other.ptr;
        AddRef();
    }

    void operator = (IntrusivePtr& other)
    {
        DecRef();

        ptr = other.ptr;

        AddRef();
    }

    IntrusivePtr(IntrusivePtr&& other)
    {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    void operator = (IntrusivePtr&& other)
    {
        DecRef();

        ptr = other.ptr;
        other.ptr = nullptr;
    }

    ~IntrusivePtr()
    {
        DecRef();
    }

private:

    void AddRef()
    {
        if (ptr)
        {
            ptr->ref_count++;
        }
    }

    void DecRef()
    {
        if (ptr)
        {
            ptr->ref_count--;
            if (ptr->ref_count == 0)
            {
                delete ptr;
            }
        }
    }

    Object* ptr = nullptr;
};
