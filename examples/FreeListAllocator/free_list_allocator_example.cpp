/////////////////////////////////////////////////////////////////////////////
// Name:        free_list_allocator_example.cpp
// Purpose:     It shows how to use FreeListAllocator
// Author:      Alexey Orlov (https://github.com/m110h)
// Created:     02/07/2020
// Licence:     MIT licence
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <random>

#include "FreeListAllocator.h"

//#define DBG_CONSTRUCTOR_DESTRUCTOR

const std::size_t MAX_AMOUNT_OF_ENTITIES {10000};

class Base
{
public:
    Base()
    {
#ifdef DBG_CONSTRUCTOR_DESTRUCTOR
        std::cout << "base constructor" << std::endl;
#endif
    }

    virtual ~Base()
    {
#ifdef DBG_CONSTRUCTOR_DESTRUCTOR
        std::cout << "base destructor" << std::endl;
#endif
    }

    virtual void Does() = 0;
};

class Child1: public Base
{
public:
    Child1(const std::string& _msg): msg(_msg)
    {
#ifdef DBG_CONSTRUCTOR_DESTRUCTOR
        std::cout << "child1 constructor" << std::endl;
#endif
    }

    virtual ~Child1()
    {
#ifdef DBG_CONSTRUCTOR_DESTRUCTOR
        std::cout << "child1 destructor" << std::endl;
#endif
    }

    virtual void Does() override
    {
        std::cout << msg << std::endl;
    }

protected:
    std::string msg {""};
};

class Child2: public Child1
{
public:
    Child2(const std::string& _msg1, const std::string& _msg2): Child1(_msg1), msg2(_msg2)
    {
#ifdef DBG_CONSTRUCTOR_DESTRUCTOR
        std::cout << "child2 constructor" << std::endl;
#endif
    }

    virtual ~Child2()
    {
#ifdef DBG_CONSTRUCTOR_DESTRUCTOR
        std::cout << "child2 destructor" << std::endl;
#endif
    }

    virtual void Does() override
    {
        std::cout << msg << msg2 << std::endl;
    }

protected:
    std::string msg2 {""};
};

class ResourceManager
{
public:
    ResourceManager(const ResourceManager& src) = delete;

    ~ResourceManager()
    {
        std::cout << "~ResourceManager()" << std::endl;
        _allocator.Reset();
    }

    mtrebi::Allocator* GetAllocator()
    {
        return &_allocator;
    }

    static ResourceManager& getInstance()
    {
        static ResourceManager _instance;
        return _instance;
    }

private:
    ResourceManager(): _allocator(sizeof(Child2)*MAX_AMOUNT_OF_ENTITIES, mtrebi::FreeListAllocator::FIND_BEST)
    {
        std::cout << "ResourceManager()" << std::endl;
        _allocator.Init();
    }

private:
    mtrebi::FreeListAllocator _allocator;
};

void TestSingleAllocation()
{
    mtrebi::Allocator* _allocator = ResourceManager::getInstance().GetAllocator();

    std::size_t aligment = 8;

    Base* b_ptr = nullptr;

    {
        Child1* c_ptr = (Child1*)_allocator->Allocate(sizeof(Child1), aligment);

        if (c_ptr)
        {
            new(c_ptr) Child1("\tChild->Does()");
            b_ptr = c_ptr;
        }
    }

    if (b_ptr)
    {
        b_ptr->Does();

        b_ptr->~Base();
        _allocator->Free(b_ptr);

        b_ptr = nullptr;
    }

    _allocator->Reset();
}

void TestMultiAllocation()
{
    std::array<Base*, MAX_AMOUNT_OF_ENTITIES> container {nullptr};

    mtrebi::Allocator* _allocator = ResourceManager::getInstance().GetAllocator();

    const std::size_t aligment = 8;

    static std::mt19937 _generator(std::chrono::system_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<int> _range(0, 1);

    std::size_t child1_counter {0};
    std::size_t child2_counter {0};
    std::size_t bad_alloc_counter {0};

    for (size_t i=0; i<container.size(); i++)
    {
        Base* b_ptr = nullptr;

        if (_range(_generator))
        {
            Child1* c_ptr = (Child1*)_allocator->Allocate(sizeof(Child1), aligment);

            if (c_ptr)
            {
                new(c_ptr) Child1("\tChild->Does()");
                b_ptr = c_ptr;

                child1_counter++;
            }
        }
        else
        {
            Child2* c_ptr = (Child2*)_allocator->Allocate(sizeof(Child2), aligment);

            if (c_ptr)
            {
                new(c_ptr) Child2("\tChild2->", "Does()");
                b_ptr = c_ptr;

                child2_counter++;
            }
        }

        if (b_ptr)
        {
            container[i] = b_ptr;
        }
        else
        {
            bad_alloc_counter++;
        }
    }
/*
    for (size_t i=0; i<container.size(); i++)
    {
        if (container[i])
            container[i]->Does();
    }
*/
    std::cout << "total memory (in bytes): " << _allocator->GetTotal() << "; used memory (in bytes): " << _allocator->GetUsed() << std::endl;

    std::cout << "amount of child1: " << child1_counter;
    std::cout << "; amount of child2: " << child2_counter;
    std::cout << "; amount of bad allocation: " << bad_alloc_counter << std::endl;

    for (size_t i=0; i<container.size(); i++)
    {
        Base* b_ptr = container[i];

        if (b_ptr)
        {
            b_ptr->~Base();
            _allocator->Free(b_ptr);
            container[i]=nullptr;
        }
    }

    std::cout << "total memory (in bytes): " << _allocator->GetTotal() << "; used memory (in bytes): " << _allocator->GetUsed() << std::endl;

    _allocator->Reset();
}

int main()
{
    TestSingleAllocation();
    TestMultiAllocation();

    return 0;
}
