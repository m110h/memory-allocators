/////////////////////////////////////////////////////////////////////////////
// Name:         StackAllocator.h
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H

#include "Allocator.h"

namespace mtrebi
{

class StackAllocator : public Allocator {
public:
    explicit StackAllocator(const std::size_t totalSize);
    ~StackAllocator();

    StackAllocator(StackAllocator &stackAllocator) = delete;
    StackAllocator& operator=(const StackAllocator& r) = delete;

    virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) final;
    virtual void Free(void* ptr) final;
    virtual void Init() final;
	virtual void Reset() final;

protected:
    void* m_start_ptr {nullptr};
    std::size_t m_offset {0};

private:
    struct AllocationHeader
    {
        char padding;
    };

};

}

#endif /* STACKALLOCATOR_H */
