/////////////////////////////////////////////////////////////////////////////
// Name:         CAllocator.cpp
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
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
void CAllocator::Reset() {}

void* CAllocator::Allocate(const std::size_t size, const std::size_t alignment)
{
	return operator new(size);
}

void CAllocator::Free(void* ptr)
{
    assert("CAllocator::Free: passed argument is NULL" && ptr);
	operator delete (ptr);
	ptr = nullptr;
}

}
