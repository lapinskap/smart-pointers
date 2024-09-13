#pragma once


struct ControlBlock {
    int ref_count = 1;
    int weakptr_count = 0;
};

// Shares ownership over an object with other Shared Ptrs that were cloned from the original one
// The idea is that the object lives as long as there's at least one SharedPtr pointing to it
// Much like with UniquePtr mixing it with raw pointers is unsafe
template<typename T>
struct SharedPtr
{
    SharedPtr() = default;

    SharedPtr(T* ptr_)
    {
        if (ptr = ptr_)
        {
            control_block = new ControlBlock;
        }
    }

    ~SharedPtr()
    {
        DecrementRef();
    }

    SharedPtr(SharedPtr& other)
    {
        ptr = other.ptr;
        control_block = other.control_block;
        IncrementRef();
    }

    SharedPtr(SharedPtr&& other)
    {
        ptr = other.ptr;
        control_block = other.control_block;

        other.control_block = nullptr;
    }

    void operator = (SharedPtr& other)
    {
        DecrementRef();
        ptr = other.ptr;
        control_block = other.control_block;
        IncrementRef();
    }

    void operator = (SharedPtr&& other)
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

    template<typename Tw>
    friend struct WeakPtr;

    void IncrementRef()
    {
        if (control_block)
        {
            control_block->ref_count++;
        }
    }

    void DecrementRef()
    {
        if (control_block)
        {
            control_block->ref_count--;

			if (control_block->ref_count == 0)
			{
				delete ptr;
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

