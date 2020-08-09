/////////////////////////////////////////////////////////////////////////////
// Name:         CAllocator.cpp
// Description:  ...
// Author:       Mariano Trebino
// Modified by:  Alexey Orlov
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#include "CAllocator.h"

#include <new>
#include <cassert>

namespace mtrebi
{

CAllocator::CAllocator(): Allocator(0) {}
CAllocator::~CAllocator(){}

void CAllocator::Init() {}

void* CAllocator::Allocate(const std::size_t size, const std::size_t alignment)
{
	return operator new(size);
}

void CAllocator::Free(void* ptr)
{
    assert("CAllocator::Free: ptr is NULL" && ptr);
	operator delete (ptr);
	ptr = nullptr;
}

}
