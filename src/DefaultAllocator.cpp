/////////////////////////////////////////////////////////////////////////////
// Name:         DefaultAllocator.cpp
// Description:  Simple wrapper around operations 'new' and 'delete'
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#include "DefaultAllocator.h"

#include <new>
#include <cassert>

namespace mtrebi
{

DefaultAllocator::DefaultAllocator(): Allocator(0) {}
DefaultAllocator::~DefaultAllocator(){}

void DefaultAllocator::Init() {}
void DefaultAllocator::Reset() {}

void* DefaultAllocator::Allocate(const std::size_t size, const std::size_t alignment)
{
    return ::operator new(size);
}

void DefaultAllocator::Free(void* ptr)
{
    assert("DefaultAllocator::Free: passed argument is NULL" && ptr);
    ::operator delete (ptr);
    ptr = nullptr;
}

}
