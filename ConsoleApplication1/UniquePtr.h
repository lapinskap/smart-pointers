#pragma once

// Unique Ptr - stupid & simple, claims 100% ownership
// Can't copy, can only move
// Don't create from raw pointers unless you know what you are doing
// Otherwise some other part of the code might try to delete it being unaware of the unique ptr
template<typename T>
struct UniquePtr
{
    UniquePtr() = default;
    UniquePtr(UniquePtr&) = delete; // No copy constructor
    void operator = (UniquePtr&) = delete; // No copy operator
    ~UniquePtr()
    {
        delete ptr;
    }

    UniquePtr(UniquePtr&& other) // Move constructor
    {
        *this = std::move(other); // Call the move operator
    }

    void operator = (UniquePtr&& other) // Move operator
    {
        // Take ownership from the other
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    T* operator -> () const // This allows our struct to behave like a pointer
    {
        return ptr;
    }

private:

    // Friend function will have access to private parts
    template<typename T>
    friend UniquePtr<T> MakeUnique();

    // No one can create a UniquePtr but ourselves and the friend
    UniquePtr(T* f)
        : ptr(f)
    {
    }

    T* ptr = nullptr;
};

// A helper function to create things wrapped into a UniquePtr - so there's no need to construct those from raw pointers
template<typename T> 
UniquePtr<T> MakeUnique()
{
    return UniquePtr<T>(new T);
}