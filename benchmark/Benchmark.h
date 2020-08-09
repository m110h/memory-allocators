/////////////////////////////////////////////////////////////////////////////
// Name:         Benchmark.h
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <vector>

#include "Allocator.h"

namespace m110h
{

// https://en.cppreference.com/w/cpp/chrono/duration/duration_cast
class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;

	using microsecond_t = std::chrono::duration<double, std::ratio<1,1000000>>;
	using millisecond_t = std::chrono::duration<double, std::ratio<1,1000>>;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_t> m_begin;

public:
    Timer() : m_begin(clock_t::now()) {}

    void Reset()
    {
        m_begin = clock_t::now();
    }

    double Elapsed() const
    {
        millisecond_t tmp = clock_t::now() - m_begin;
        return tmp.count();
    }
};

}

namespace mtrebi
{

struct BenchmarkResults
{
    long nOperations {0};
    double elapsedTime {0};
    //double operationsPerSec {0};
    //double timePerOperation {0};
    std::size_t memoryPeak {0};
};

class Benchmark {
public:
	Benchmark(unsigned int nOperations);

	void SingleAllocation(Allocator* allocator, const std::size_t size, const std::size_t alignment);
	void SingleFree(Allocator* allocator, const std::size_t size, const std::size_t alignment);

	void MultipleAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments);
	void MultipleFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments);

	void RandomAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments);
	void RandomFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments);
private:
	void PrintResults(const BenchmarkResults& results) const;
	void RandomAllocationAttr(const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments, std::size_t & size, std::size_t & alignment);

	const BenchmarkResults BuildResults(unsigned int nOperations, double elapsedTime, std::size_t memoryUsed) const;
private:
	unsigned int m_nOperations;
	m110h::Timer m_timer;
};

}

#endif /* BENCHMARK_H */
