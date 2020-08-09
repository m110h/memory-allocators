/////////////////////////////////////////////////////////////////////////////
// Name:         PoolAllocator.cpp
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#include "PoolAllocator.h"

#include <cassert>
#include <algorithm>    //max
#include <new>

#ifdef _DEBUG_OUTPUT_
#include <iostream>
#endif

namespace mtrebi
{

PoolAllocator::PoolAllocator(const std::size_t totalSize, const std::size_t chunkSize): Allocator(totalSize), m_chunkSize(chunkSize)
{
    assert("PoolAllocator::PoolAllocator: Chunk size must be greater or equal to 8" && (chunkSize >= 8));
    assert("PoolAllocator::PoolAllocator: Total Size must be a multiple of Chunk Size" && (totalSize % chunkSize == 0));
}

void PoolAllocator::Init()
{
    if (m_start_ptr)
    {
        operator delete (m_start_ptr);
        m_start_ptr = nullptr;
    }

    m_start_ptr = operator new(m_totalSize);

    Reset();
}

PoolAllocator::~PoolAllocator()
{
    if (m_start_ptr != nullptr)
    {
        operator delete (m_start_ptr);
        m_start_ptr = nullptr;
    }
}

void* PoolAllocator::Allocate(const std::size_t allocationSize, const std::size_t alignment)
{
    assert("PoolAllocator::Allocate: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);
    assert("PoolAllocator::Allocate: Allocation size must be equal to chunk size" && (allocationSize == m_chunkSize));

    Node* freePosition = m_freeList.pop();

    //assert(freePosition != nullptr && "PoolAllocator::Allocate: The pool allocator is full");
    if (freePosition == nullptr)
    {
        return nullptr;
    }

    m_used += m_chunkSize;
    m_peak = std::max(m_peak, m_used);
#ifdef _DEBUG_OUTPUT_
    std::cout << "A" << "\t@S " << m_start_ptr << "\t@R " << (void*) freePosition << "\tM " << m_used << std::endl;
#endif

    return reinterpret_cast<void*>(freePosition);
}

void PoolAllocator::Free(void* ptr)
{
    assert("PoolAllocator::Free: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);
    assert("PoolAllocator::Free: passed argument is NULL" && ptr);

    assert("PoolAllocator::Free: m_used < m_chunkSize" && m_used >= m_chunkSize);
    m_used -= m_chunkSize;

    m_freeList.push(reinterpret_cast<Node*>(ptr));

#ifdef _DEBUG_OUTPUT_
    std::cout << "F" << "\t@S " << m_start_ptr << "\t@F " << ptr << "\tM " << m_used << std::endl;
#endif
}

void PoolAllocator::Reset()
{
    assert("PoolAllocator::Reset: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);

    m_used = 0;
    m_peak = 0;

    // Create a linked-list with all free positions
    const int nChunks = m_totalSize / m_chunkSize;

    for (int i = 0; i < nChunks; ++i)
    {
        std::size_t address = reinterpret_cast<std::size_t>(m_start_ptr) + i*m_chunkSize;
        m_freeList.push(reinterpret_cast<Node*>(address));
    }
}

}
