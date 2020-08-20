/////////////////////////////////////////////////////////////////////////////
// Name:         HeapAllocator.h
// Description:  Fast custom memory allocator based on Binary Heap
// Author:       Alexey Orlov (https://github.com/m110h)
// Created:      18/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef HEAPALLOCATOR_H
#define HEAPALLOCATOR_H

#include "Allocator.h"

namespace mtrebi
{

class Heap;

class HeapAllocator : public Allocator
{
public:
	explicit HeapAllocator(std::size_t totalSize, std::size_t maxEntities = 1000);

	HeapAllocator(HeapAllocator &src) = delete;
	HeapAllocator& operator=(const HeapAllocator& r) = delete;

    ~HeapAllocator();

	virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) final;
	virtual void Free(void* ptr) final;
	virtual void Init() final;
	virtual void Reset() final;

public:
    struct BlockHeader
    {
        std::size_t blockSize {0};

        friend bool operator<(const BlockHeader& left, const BlockHeader& right);
        friend bool operator>(const BlockHeader& left, const BlockHeader& right);
        friend bool operator<(const std::size_t& left, const BlockHeader& right);
        friend bool operator>(const std::size_t& left, const BlockHeader& right);
    };

private:
    std::size_t m_heap_capacity {0};

	void* m_start_ptr {nullptr};
	Heap* m_heap {nullptr};
};

class Heap
{
public:
    explicit Heap(std::size_t _capacity);
    ~Heap();

    bool Insert(HeapAllocator::BlockHeader* _header);
    HeapAllocator::BlockHeader* Extract(std::size_t allocationSize);

    std::size_t GetSize() const;
    bool IsFull() const;
    void Clear();

private:
    std::size_t Parent(std::size_t i) const;
    std::size_t Left(std::size_t i) const;
    std::size_t Right(std::size_t i) const;

    void SiftUp(std::size_t i);
    void SiftDown(std::size_t i);

    void Swap(HeapAllocator::BlockHeader* h1, HeapAllocator::BlockHeader* h2);

private:
    HeapAllocator::BlockHeader** container {nullptr};

    std::size_t m_size {0};
    std::size_t m_capacity {0};
};

}

#endif /* HEAPALLOCATOR_H */

