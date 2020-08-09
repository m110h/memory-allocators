/////////////////////////////////////////////////////////////////////////////
// Name:         main.cpp
// Description:  The main entry point of Benchmark
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

#include "Benchmark.h"
#include "Allocator.h"
#include "StackAllocator.h"
#include "CAllocator.h"
#include "LinearAllocator.h"
#include "PoolAllocator.h"
#include "FreeListAllocator.h"

int main()
{
    const unsigned nOperations = 10000;
    const std::size_t regionSize = (4096*2)*nOperations;

    const std::vector<std::size_t> ALLOCATION_SIZES {32, 64, 256, 512, 1024, 2048, 4096};
    const std::vector<std::size_t> ALIGNMENTS {8, 8, 8, 8, 8, 8, 8};

    mtrebi::CAllocator cAllocator;
    mtrebi::LinearAllocator linearAllocator(regionSize);
    mtrebi::StackAllocator stackAllocator(regionSize);
    mtrebi::PoolAllocator poolAllocator(regionSize, 4096);
    mtrebi::FreeListAllocator freeListAllocator(regionSize, mtrebi::FreeListAllocator::PlacementPolicy::FIND_BEST);

    mtrebi::Benchmark benchmark(nOperations);

    std::cout << "C" << std::endl;
    //benchmark.MultipleAllocation(static_cast<mtrebi::Allocator*>(&cAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    benchmark.MultipleFree(static_cast<mtrebi::Allocator*>(&cAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    //benchmark.RandomAllocation(static_cast<mtrebi::Allocator*>(&cAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    //benchmark.RandomFree(static_cast<mtrebi::Allocator*>(&cAllocator), ALLOCATION_SIZES, ALIGNMENTS);

    std::cout << "LINEAR" << std::endl;
    benchmark.MultipleAllocation(static_cast<mtrebi::Allocator*>(&linearAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    //benchmark.RandomAllocation(static_cast<mtrebi::Allocator*>(&linearAllocator), ALLOCATION_SIZES, ALIGNMENTS);

    std::cout << "STACK" << std::endl;
    //benchmark.MultipleAllocation(static_cast<mtrebi::Allocator*>(&stackAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    benchmark.MultipleFree(static_cast<mtrebi::Allocator*>(&stackAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    //benchmark.RandomAllocation(static_cast<mtrebi::Allocator*>(&stackAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    //benchmark.RandomFree(static_cast<mtrebi::Allocator*>(&stackAllocator), ALLOCATION_SIZES, ALIGNMENTS);

    //std::cout << "POOL" << std::endl;
    //benchmark.SingleAllocation(static_cast<mtrebi::Allocator*>(&poolAllocator), 4096, 8);
    //benchmark.SingleFree(static_cast<mtrebi::Allocator*>(&poolAllocator), 4096, 8);

    std::cout << "FREE LIST" << std::endl;
    //benchmark.MultipleAllocation(static_cast<mtrebi::Allocator*>(&freeListAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    benchmark.MultipleFree(static_cast<mtrebi::Allocator*>(&freeListAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    //benchmark.RandomAllocation(static_cast<mtrebi::Allocator*>(&freeListAllocator), ALLOCATION_SIZES, ALIGNMENTS);
    //benchmark.RandomFree(static_cast<mtrebi::Allocator*>(&freeListAllocator), ALLOCATION_SIZES, ALIGNMENTS);

    return 0;
}
