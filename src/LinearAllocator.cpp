/////////////////////////////////////////////////////////////////////////////
// Name:         LinearAllocator.cpp
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#include "LinearAllocator.h"
#include "Utils.h"  /* CalculatePadding */

#include <cassert>
#include <algorithm>
#include <new>

#ifdef _DEBUG_OUTPUT_
#include <iostream>
#endif

namespace mtrebi
{

LinearAllocator::LinearAllocator(const std::size_t totalSize): Allocator(totalSize) {}

LinearAllocator::~LinearAllocator()
{
    if (m_start_ptr != nullptr)
    {
        operator delete(m_start_ptr);
        m_start_ptr = nullptr;
    }
}

void LinearAllocator::Init()
{
    if (m_start_ptr != nullptr)
    {
        operator delete(m_start_ptr);
        m_start_ptr = nullptr;
    }

    m_start_ptr = operator new(m_totalSize);
    m_offset = 0;
}

void* LinearAllocator::Allocate(const std::size_t size, const std::size_t alignment)
{
    assert("LinearAllocator::Allocate: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);

    std::size_t padding = 0;
    std::size_t paddedAddress = 0;

    const std::size_t currentAddress = reinterpret_cast<std::size_t>(m_start_ptr) + m_offset;

    if ( (alignment != 0) && (m_offset % alignment != 0) )
    {
        // Alignment is required. Find the next aligned memory address and update offset
        padding = Utils::CalculatePadding(currentAddress, alignment);
    }

    if ( (m_offset + padding + size) > m_totalSize )
    {
        return nullptr;
    }

    m_offset += padding;
    m_offset += size;

    const std::size_t nextAddress = currentAddress + padding;
#ifdef _DEBUG_OUTPUT_
    std::cout << "A" << "\t@C " << (void*) currentAddress << "\t@R " << (void*) nextAddress << "\tO " << m_offset << "\tP " << padding << std::endl;
#endif

    m_used = m_offset;
    m_peak = std::max(m_peak, m_used);

    return reinterpret_cast<void*>(nextAddress);
}

void LinearAllocator::Free(void* ptr)
{
    assert(false && "LinearAllocator::Free: method Free() isn't allowed. use Reset() instead");
}

void LinearAllocator::Reset()
{
    assert("LinearAllocator::Reset: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);

    m_offset = 0;
    m_used = 0;
    m_peak = 0;
}

}
