/////////////////////////////////////////////////////////////////////////////
// Name:         HeapAllocator.cpp
// Description:  Fast custom memory allocator based on Binary Heap
// Author:       Alexey Orlov (https://github.com/m110h)
// Created:      18/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#include "HeapAllocator.h"

#include <new>
#include <cassert>

namespace mtrebi
{

// BlockHeader Impl

bool operator<(const HeapAllocator::BlockHeader& left, const HeapAllocator::BlockHeader& right)
{
    return ( left.blockSize < right.blockSize );
}

bool operator>(const HeapAllocator::BlockHeader& left, const HeapAllocator::BlockHeader& right)
{
    return ( left.blockSize > right.blockSize );
}

bool operator<(const std::size_t& left, const HeapAllocator::BlockHeader& right)
{
    return ( left < right.blockSize );
}

bool operator>(const std::size_t& left, const HeapAllocator::BlockHeader& right)
{
    return ( left > right.blockSize );
}

// Heap Impl

Heap::Heap(std::size_t _capacity): m_size(0), m_capacity(_capacity)
{
    container = new HeapAllocator::BlockHeader* [_capacity];
}

Heap::~Heap()
{
    delete [] container; container = nullptr;
}

bool Heap::Insert(HeapAllocator::BlockHeader* _header)
{
    if (IsFull())
    {
        return false;
    }

    m_size++;
    std::size_t i = m_size - 1;

    container[i] = _header;

    SiftUp(i);

    return true;
}

// log(n)
HeapAllocator::BlockHeader* Heap::Extract(std::size_t allocationSize)
{
    //assert(size > 0 && "Heap is empty");
    if (m_size == 0)
    {
        return nullptr;
    }

    if (m_size == 1)
    {
        if (allocationSize < *container[0])
        {
            m_size = 0;
            return container[0];
        }

        return nullptr;
    }

    std::size_t i = 0;

    while ( (i < m_size) && (allocationSize > *container[i]))
    {
        std::size_t tmp = i;

        std::size_t l = Left(i);
        std::size_t r = Right(i);

        if ( (l < m_size) && (*container[tmp] < *container[l]) )
            tmp = l;
        if ( (r < m_size) && (*container[tmp] < *container[r]) )
            tmp = r;

        if (tmp == i)
        {
            return nullptr;
        }

        i = tmp;
    }

    HeapAllocator::BlockHeader* blockPtr = container[i];

    container[i] = container[m_size-1];
    m_size--;
    SiftDown(i);

    return blockPtr;
}

std::size_t Heap::GetSize() const { return m_size; }

bool Heap::IsFull() const
{
    return m_size == m_capacity;
}

void Heap::Clear()
{
    for (std::size_t i=0; i<m_size; i++)
        container[i] = nullptr;

    m_size = 0;
}

std::size_t Heap::Parent(std::size_t i) const { return (i-1)/2; }
std::size_t Heap::Left(std::size_t i) const { return 2*i + 1; }
std::size_t Heap::Right(std::size_t i) const { return 2*i + 2; }

void Heap::SiftUp(std::size_t i)
{
    while ( (i != 0) && (*container[i] < *container[Parent(i)]) )
    {
        Swap(container[Parent(i)], container[i]);
        i = Parent(i);
    }
}

void Heap::SiftDown(std::size_t i)
{
    std::size_t l, r, tmp;

    for (;;)
    {
        tmp = i;

        l = Left(i);
        r = Right(i);

        if ( (l < m_size) && (*container[tmp] < *container[l]) )
        {
            tmp = l;
        }

        if ( (r < m_size) && (*container[tmp] < *container[r]) )
        {
            tmp = r;
        }

        if (tmp == i)
        {
            break;
        }

        Swap(container[i], container[tmp]);
        i = tmp;
    }
}

void Heap::Swap(HeapAllocator::BlockHeader* h1, HeapAllocator::BlockHeader* h2)
{
    HeapAllocator::BlockHeader* tmp = h1;
    h1 = h2;
    h2 = tmp;
}

// HeapAllocator Impl


HeapAllocator::HeapAllocator(std::size_t totalSize, std::size_t maxEntities):
    Allocator(totalSize + (maxEntities + 1)*sizeof(BlockHeader)), m_heap_capacity(maxEntities + 1) {}

HeapAllocator::~HeapAllocator()
{
    if (m_start_ptr != nullptr)
    {
        ::operator delete (m_start_ptr);
        m_start_ptr = nullptr;
    }

    if (m_heap != nullptr)
    {
        delete m_heap;
        m_heap = nullptr;
    }
}

void HeapAllocator::Init()
{
    if (m_start_ptr != nullptr)
    {
        ::operator delete(m_start_ptr);
        m_start_ptr = nullptr;
    }

    m_start_ptr = ::operator new(m_totalSize);

    if (m_heap != nullptr)
    {
        delete m_heap;
        m_heap = nullptr;
    }

    m_heap = new Heap(m_heap_capacity);

    Reset();
}

void HeapAllocator::Reset()
{
    assert("HeapAllocator::Reset: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);
    assert("HeapAllocator::Reset: allocator isn't initialized, m_heap is NULL" && m_heap);

    m_heap->Clear();

    BlockHeader* header = reinterpret_cast<BlockHeader*>(m_start_ptr);
    header->blockSize = m_totalSize;

    assert("HeapAllocator::Reset: bad insert to heap" && m_heap->Insert(header));
    m_used = m_peak = 0;
}

void* HeapAllocator::Allocate(const std::size_t size, const std::size_t alignment)
{
    assert("HeapAllocator::Allocate: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);
    assert("HeapAllocator::Allocate: allocator isn't initialized, m_heap is NULL" && m_heap);

    std::size_t requiredSize = sizeof(BlockHeader) + size;

    if ( (requiredSize % alignment) > 0 )
    {
        requiredSize += alignment - (requiredSize % alignment);
    }

    BlockHeader* header = m_heap->Extract(requiredSize);

    if (header)
    {
        if ( header->blockSize > (requiredSize + sizeof(BlockHeader)) )
        {
            BlockHeader* freeBlock = reinterpret_cast<BlockHeader*>(reinterpret_cast<std::size_t>(header) + requiredSize);

            freeBlock->blockSize = header->blockSize - requiredSize;
            header->blockSize = requiredSize;

            assert("HeapAllocator::Allocate: bad insert to heap" && m_heap->Insert(freeBlock));
        }

        m_used += header->blockSize;
        m_peak = (m_peak < m_used) ? m_used : m_peak;

        return reinterpret_cast<void*>(reinterpret_cast<std::size_t>(header) + sizeof(BlockHeader));
    }

    return nullptr;
}

void HeapAllocator::Free(void* ptr)
{
    assert("HeapAllocator::Free: allocator isn't initialized, m_start_ptr is NULL" && m_start_ptr);
    assert("HeapAllocator::Free: allocator isn't initialized, m_heap is NULL" && m_heap);
    assert("HeapAllocator::Free: passed argument is NULL" && ptr);

    BlockHeader* header = reinterpret_cast<BlockHeader*>(reinterpret_cast<std::size_t>(ptr) - sizeof(BlockHeader));
    m_used -= header->blockSize;

    assert("HeapAllocator::Free: bad insert to heap" && m_heap->Insert(header));
}

}
