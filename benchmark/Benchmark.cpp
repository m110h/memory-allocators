/////////////////////////////////////////////////////////////////////////////
// Name:         Benchmark.cpp
// Description:  ...
// Author:       Mariano Trebino
// Modified by:  Alexey Orlov
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#include "Benchmark.h"

#include <iostream>
#include <random>
#include <thread>
#include <cassert>

namespace mtrebi
{

Benchmark::Benchmark(unsigned int nOperations): m_nOperations(nOperations) {}

void Benchmark::SingleAllocation(Allocator* allocator, const std::size_t size, const std::size_t alignment)
{
    allocator->Init();
    unsigned int operations = 0;

    m_timer.Reset();

    while (operations < m_nOperations)
    {
        allocator->Allocate(size, alignment);
        ++operations;
    }

    double _delta = m_timer.Elapsed();

    BenchmarkResults results = BuildResults(m_nOperations, _delta, allocator->GetPeak());
    PrintResults(results);
}

void Benchmark::SingleFree(Allocator* allocator, const std::size_t size, const std::size_t alignment)
{
    void* addresses[m_nOperations];

    allocator->Init();
    int operations = 0;

    m_timer.Reset();

    while (operations < m_nOperations)
    {
        addresses[operations] = allocator->Allocate(size, alignment);
        ++operations;
    }
    --operations;
    while (operations >= 0)
    {
        if (addresses[operations])
            allocator->Free(addresses[operations]);
        --operations;
    }

    double _delta = m_timer.Elapsed();

    BenchmarkResults results = BuildResults(m_nOperations, _delta, allocator->GetPeak());
    PrintResults(results);
}

void Benchmark::MultipleAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments) {
    assert(allocationSizes.size() == alignments.size() && "Allocation sizes and Alignments must have same length");

    for (int i = 0; i < allocationSizes.size(); ++i)
    {
        SingleAllocation(allocator, allocationSizes[i], alignments[i]);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Benchmark::MultipleFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments) {
    assert(allocationSizes.size() == alignments.size() && "Allocation sizes and Alignments must have same length");

    for (int i = 0; i < allocationSizes.size(); ++i)
    {
        SingleFree(allocator, allocationSizes[i], alignments[i]);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Benchmark::RandomAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments)
{
    std::cout << "\tBENCHMARK: ALLOCATION" << std::endl;

    std::size_t allocation_size;
    std::size_t alignment;

    allocator->Init();
    unsigned int operations = 0;

    m_timer.Reset();

    while (operations < m_nOperations)
    {
        RandomAllocationAttr(allocationSizes, alignments, allocation_size, alignment);
        allocator->Allocate(allocation_size, alignment);
        ++operations;
    }

    double _delta = m_timer.Elapsed();

    BenchmarkResults results = BuildResults(m_nOperations, _delta, allocator->GetPeak());
    PrintResults(results);

}

void Benchmark::RandomFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments)
{
    std::cout << "\tBENCHMARK: ALLOCATION/FREE" << std::endl;

    void* addresses[m_nOperations];

    std::size_t allocation_size;
    std::size_t alignment;

    allocator->Init();
    int operations = 0;

    m_timer.Reset();

    while (operations < m_nOperations)
    {
        RandomAllocationAttr(allocationSizes, alignments, allocation_size, alignment);
        addresses[operations] = allocator->Allocate(allocation_size, alignment);
        ++operations;
    }
    --operations;
    while (operations >= 0)
    {
        if (addresses[operations])
            allocator->Free(addresses[operations]);
        --operations;
    }

    double _delta = m_timer.Elapsed();

    BenchmarkResults results = BuildResults(m_nOperations, _delta, allocator->GetPeak());
    PrintResults(results);

}

void Benchmark::PrintResults(const BenchmarkResults& results) const
{
    std::cout << "\t\tOperations:    \t" << results.nOperations << std::endl;
    std::cout << "\t\tTime elapsed:  \t" << results.elapsedTime << " ms" << std::endl;
    //std::cout << "\t\tOp per sec:    \t" << results.operationsPerSec << " ops/ms" << std::endl;
    //std::cout << "\t\tTimer per op:  \t" << results.timePerOperation << " ms/ops" << std::endl;
    std::cout << "\t\tMemory peak:   \t" << results.memoryPeak << " bytes" << std::endl;

    std::cout << std::endl;
}

const BenchmarkResults Benchmark::BuildResults(unsigned int nOperations, double elapsedTime, std::size_t memoryPeak) const
{
    BenchmarkResults results;

    results.nOperations = nOperations;
    results.elapsedTime = elapsedTime;
    //results.operationsPerSec = results.nOperations / results.elapsedTime;
    //results.timePerOperation = results.elapsedTime / results.nOperations;
    results.memoryPeak = memoryPeak;

    return results;
}

void Benchmark::RandomAllocationAttr(
    const std::vector<std::size_t>& allocationSizes,
    const std::vector<std::size_t>& alignments,
    std::size_t & size,
    std::size_t & alignment)
{
    static std::mt19937 _generator(std::chrono::system_clock::now().time_since_epoch().count());

    std::uniform_int_distribution<int> range(0, allocationSizes.size()-1);
    const int _index = range(_generator);

    size = allocationSizes[_index];
    alignment = alignments[_index];
}

}
