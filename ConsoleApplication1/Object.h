#pragma once
struct Object
{
    virtual ~Object() = default;

private:

    friend struct IntrusivePtr;

    int ref_count = 0;

};