#pragma once
#include "SharedPtr.h";

template<typename T>
struct WeakPtr
{
    WeakPtr() = default;

    ~WeakPtr()
    {
        DecrementRef();
    }

    WeakPtr(const SharedPtr<T>& other)
    {
        ptr = other.ptr;
        control_block = other.control_block;
		IncrementRef();
    }

    WeakPtr(const WeakPtr& other)
    {
        ptr = other.ptr;
        control_block = other.control_block;
        IncrementRef();
    }

    WeakPtr(WeakPtr&& other)
    {
        ptr = other.ptr;
        control_block = other.control_block;
        other.control_block = nullptr;
    }

    void operator = (WeakPtr& other)
    {
        DecrementRef();
        ptr = other.ptr;
        control_block = other.control_block;
        IncrementRef();
    }

    void operator = (WeakPtr&& other)
    {
        DecrementRef();
        ptr = other.ptr;
        control_block = other.control_block;
        other.control_block = nullptr;
    }

    T* operator -> () const // This allows our struct to behave like a pointer
    {
        return ptr;
    }


private:

    void IncrementRef()
    {
        if (control_block)
        {
            control_block->weakptr_count++;
        }
    }

    void DecrementRef()
    {
        if (control_block)
        {
            control_block->weakptr_count--;

            if (control_block->ref_count == 0)
            {
                ptr = nullptr;

                if (control_block->weakptr_count == 0)
                {
                    delete control_block;
                }
            }
        }
    }

    T* ptr = nullptr;
    ControlBlock* control_block = nullptr;
};

