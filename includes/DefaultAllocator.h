/////////////////////////////////////////////////////////////////////////////
// Name:         DefaultAllocator.h
// Description:  Simple wrapper around operations 'new' and 'delete'
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef DEFAULT_ALLOCATOR_H
#define DEFAULT_ALLOCATOR_H

#include "Allocator.h"

namespace mtrebi
{

class DefaultAllocator : public Allocator
{
public:
	DefaultAllocator();
    ~DefaultAllocator();

	virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) final;
	virtual void Free(void* ptr) final;
	virtual void Init() final;
	virtual void Reset() final;
};

}

#endif /* DEFAULT_ALLOCATOR_H */

