/////////////////////////////////////////////////////////////////////////////
// Name:         StackAllocator.cpp
// Description:  ...
// Author:       Mariano Trebino
// Modified by:  Alexey Orlov
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#include "StackAllocator.h"
#include "Utils.h"  /* CalculatePadding */

#include <algorithm>
#include <new>
#include <cassert>
#include <limits>

#ifdef _DEBUG_OUTPUT_
#include <iostream>
#endif

namespace mtrebi
{

StackAllocator::StackAllocator(const std::size_t totalSize): Allocator(totalSize) {}

StackAllocator::~StackAllocator()
{
    if (m_start_ptr)
    {
        operator delete (m_start_ptr);
        m_start_ptr = nullptr;
    }
}

void StackAllocator::Init()
{
    if (m_start_ptr)
    {
        operator delete (m_start_ptr);
        m_start_ptr = nullptr;
    }

    m_start_ptr = operator new(m_totalSize);
    m_offset = 0;
}

void* StackAllocator::Allocate(const std::size_t size, const std::size_t alignment)
{
    assert("StackAllocator::Allocate: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);

    const std::size_t currentAddress = reinterpret_cast<std::size_t>(m_start_ptr) + m_offset;

    std::size_t padding = Utils::CalculatePaddingWithHeader(currentAddress, alignment, sizeof(AllocationHeader));

    if ( (m_offset + padding + size) > m_totalSize )
    {
        return nullptr;
    }

    m_offset += padding;

    const std::size_t nextAddress = currentAddress + padding;

    assert("StackAllocator::Allocate: nextAddress < sizeof(AllocationHeader)" && nextAddress >= sizeof(AllocationHeader));
    const std::size_t headerAddress = nextAddress - sizeof(AllocationHeader);

    AllocationHeader* headerPtr = reinterpret_cast<AllocationHeader*>(headerAddress);
    assert("StackAllocator::Allocate: padding > std::numeric_limits<char>::max()" && padding <= std::numeric_limits<char>::max());
    // std::size_t to char (!)
    headerPtr->padding = padding;

    m_offset += size;

#ifdef _DEBUG_OUTPUT_
    std::cout << "A" << "\t@C " << (void*) currentAddress << "\t@R " << (void*) nextAddress << "\tO " << m_offset << "\tP " << padding << std::endl;
#endif
    m_used = m_offset;
    m_peak = std::max(m_peak, m_used);

    return reinterpret_cast<void*>(nextAddress);
}

void StackAllocator::Free(void *ptr)
{
    assert("StackAllocator::Free: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);
    assert("StackAllocator::Free: passed argument is NULL" && ptr);

    // Move offset back to clear address
    const std::size_t currentAddress = reinterpret_cast<std::size_t>(ptr);

    assert("StackAllocator::Free: currentAddress < sizeof(AllocationHeader)" && currentAddress >= sizeof(AllocationHeader));
    const std::size_t headerAddress = currentAddress - sizeof(AllocationHeader);

    const AllocationHeader* allocationHeader = reinterpret_cast<AllocationHeader*>(headerAddress);

    assert("StackAllocator::Free: currentAddress < (allocationHeader->padding + reinterpret_cast<std::size_t>(m_start_ptr))" && ( currentAddress >= (allocationHeader->padding + reinterpret_cast<std::size_t>(m_start_ptr)) ) );
    m_offset = currentAddress - (allocationHeader->padding + reinterpret_cast<std::size_t>(m_start_ptr));
    m_used = m_offset;

#ifdef _DEBUG_OUTPUT_
    std::cout << "F" << "\t@C " << (void*) currentAddress << "\t@F " << (void*) ((char*) m_start_ptr + m_offset) << "\tO " << m_offset << std::endl;
#endif
}

void StackAllocator::Reset()
{
    assert("StackAllocator::Reset: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);

    m_offset = 0;
    m_used = 0;
    m_peak = 0;
}

}
