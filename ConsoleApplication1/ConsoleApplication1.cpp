#include <iostream>
#include <memory>
#include <assert.h>
#include "Object.h"
#include "UniquePtr.h"
#include "IntrusivePtr.h"
#include "SharedPtr.h"
#include "WeakPtr.h"

int counter = 0;

struct Foo : public Object
{
    Foo()
    {
        counter++;
        std::cout << "Foo mowi czemsc" << std::endl;
    }
    ~Foo()
    {
        assert(counter > 0);
        counter--;
        std::cout << "Foo ded [*]" << std::endl;
    }

    char data[64];
};


struct Foo2
{
    Foo2()
    {
        counter++;
    }

    ~Foo2()
    {
        counter--;
    }

    SharedPtr<Foo2> other;
    WeakPtr<Foo2> other_weak;
};

int main()
{
    // RAII - Resource Allocation Is Initialization
    // 

    {
        UniquePtr<Foo> p1 = MakeUnique<Foo>(); 
        assert(counter == 1);
        UniquePtr<Foo> p2 = std::move(p1);
        assert(counter == 1);
    }

    assert(counter == 0);

    {
        
        std::cout << "tududu" << std::endl;
        assert(counter == 0);

    }

    // STD version
    {
        std::unique_ptr<Foo> p1 = std::make_unique<Foo>();
        assert(counter == 1);
        std::unique_ptr<Foo> p2 = std::move(p1);
        assert(counter == 1);
    }
    assert(counter == 0);

    {
        IntrusivePtr p1 = new Foo;
        assert(counter == 1);
        IntrusivePtr p2 = new Foo;
        assert(counter == 2);
        p1 = p2;
        assert(counter == 1);
    }
    assert(counter == 0);

    {
        SharedPtr<Foo> p1 = new Foo;
        assert(counter == 1);
        SharedPtr<Foo> p2 = new Foo;
        assert(counter == 2);
        p1 = p2;
        assert(counter == 1);
    }

    {
        Foo* raw_pointer = new Foo;
        SharedPtr<Foo> p3 = raw_pointer;
        //SharedPtr<Foo> p4 = raw_pointer; ERROR!
        SharedPtr<Foo> p4 = p3; // OK
    }
    assert(counter == 0);

    {
        SharedPtr<Foo2> p1 = new Foo2;
        SharedPtr<Foo2> p2 = new Foo2;
        p1->other = p2;
        p2->other_weak = p1;
    }
    assert(counter == 0);

}
