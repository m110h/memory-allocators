/////////////////////////////////////////////////////////////////////////////
// Name:         PoolAllocator.h
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#include "Allocator.h"
#include "StackLinkedList.h"

namespace mtrebi
{

class PoolAllocator : public Allocator
{
private:
    struct  FreeHeader{};

    using Node = StackLinkedList<FreeHeader>::Node;
    StackLinkedList<FreeHeader> m_freeList;

    void* m_start_ptr {nullptr};
    std::size_t m_chunkSize;

public:
    PoolAllocator(const std::size_t totalSize, const std::size_t chunkSize);

    PoolAllocator(PoolAllocator &src) = delete;
    PoolAllocator& operator=(const PoolAllocator& r) = delete;

    ~PoolAllocator();

    virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) final;
    virtual void Free(void* ptr) final;
    virtual void Init() final;
	virtual void Reset() final;
};

}
