/////////////////////////////////////////////////////////////////////////////
// Name:         LinearAllocator.h
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef LINEARALLOCATOR_H
#define LINEARALLOCATOR_H

#include "Allocator.h"

namespace mtrebi
{

class LinearAllocator : public Allocator
{
public:
	explicit LinearAllocator(const std::size_t totalSize);

	LinearAllocator(LinearAllocator &linearAllocator) = delete;
	LinearAllocator& operator=(const LinearAllocator& r) = delete;

	~LinearAllocator();

	virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) final;
	virtual void Free(void* ptr) final;
	virtual void Init() final;
	virtual void Reset() final;

protected:
	void* m_start_ptr {nullptr};
	std::size_t m_offset {0};
};

}

#endif /* LINEARALLOCATOR_H */
