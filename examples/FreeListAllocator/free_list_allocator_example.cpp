/////////////////////////////////////////////////////////////////////////////
// Name:        free_list_allocator_example.cpp
// Purpose:     It shows how to use FreeListAllocator
// Author:      Alexey Orlov
// Created:     02/07/2020
// Licence:     MIT licence
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <array>

#include "FreeListAllocator.h"

#define DBG_CONSTRUCTOR_DESTRUCTOR

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

class Child: public Base
{
public:
    Child(const std::string& _msg)
    {
#ifdef DBG_CONSTRUCTOR_DESTRUCTOR
        std::cout << "child constructor" << std::endl;
#endif
        msg = _msg;
    }

    virtual ~Child()
    {
#ifdef DBG_CONSTRUCTOR_DESTRUCTOR
        std::cout << "child destructor" << std::endl;
#endif
    }

    virtual void Does() override
    {
        std::cout << msg << std::endl;
    }

private:
    std::string msg {""};
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
    ResourceManager(): _allocator(sizeof(Child)*10000, mtrebi::FreeListAllocator::FIND_BEST)
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

    Base* b_ptr = nullptr;

    {
        Child* c_ptr = (Child*)_allocator->Allocate(sizeof(Child),8);

        if (c_ptr)
        {
            new(c_ptr) Child("\tChild->Does()");
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
}

void TestMultiAllocation()
{
    std::array<Base*,5000> container {nullptr};

    mtrebi::Allocator* _allocator = ResourceManager::getInstance().GetAllocator();

    for (size_t i=0; i<container.size(); i++)
    {
        Base* b_ptr = nullptr;

        {
            Child* c_ptr = (Child*)_allocator->Allocate(sizeof(Child),8);

            if (c_ptr)
            {
                new(c_ptr) Child("\tChild->Does()");
                b_ptr = c_ptr;
            }
            else
            {
                std::cout << "Not enough memory" << std::endl;
            }
        }

        container[i] = b_ptr;
    }
/*
    for (size_t i=0; i<container.size(); i++)
    {
        if (container[i])
            container[i]->Does();
    }
*/
    std::cout << "total: " << _allocator->GetTotal() << " used: " << _allocator->GetUsed() << std::endl;

    //system("pause");

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

    std::cout << "total: " << _allocator->GetTotal() << " used: " << _allocator->GetUsed() << std::endl;
}

int main()
{
    TestSingleAllocation();
    //TestMultiAllocation();

    return 0;
}
