/////////////////////////////////////////////////////////////////////////////
// Name:         CAllocator.h
// Description:  ...
// Author:       Mariano Trebino
// Modified by:  Alexey Orlov
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef CALLOCATOR_H
#define CALLOCATOR_H

#include "Allocator.h"

namespace mtrebi
{

class CAllocator : public Allocator
{
public:
	CAllocator();
    ~CAllocator();

	virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) final;
	virtual void Free(void* ptr) final;
	virtual void Init() final;
};

}

#endif /* CALLOCATOR_H */

