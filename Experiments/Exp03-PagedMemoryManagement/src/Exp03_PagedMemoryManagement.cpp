#include <iostream>
#include <format>
#include <functional>
#include "Random.h"
#include "PagedMemory.h"

std::vector<uint32_t> GeneratePageReferences(uint32_t refCount)
{
    std::vector<uint32_t> pageRefs(refCount);
    for (int i = 0; i < refCount; ++i)
        pageRefs[i] = Random::GenerateInt32(0, 6);
    return pageRefs;
}

void PrintMemoryState(const std::vector<std::vector<uint32_t>>& memoryState, int capacity)
{
    for (int i = 0; i < capacity; ++i)
    {
        for (int j = 0; j < memoryState.size(); ++j)
        {
            if (i < memoryState[j].size())
                std::cout << std::format("{:>2} ", memoryState[j][i]);
            else
                std::cout << "   ";
        }
        std::cout << "\n";
    }
}

int main()
{
    Random::Init();

    // Initialize memory.
    PagedMemory memory;
    uint32_t memorySize = 48, pageSize = 16;
    memory.SetVirtualMemorySize(memorySize);
    memory.SetPageSize(pageSize);
    uint32_t capacity = memorySize / pageSize;

    // Generate page references.
    auto pageRefs = GeneratePageReferences(20);
    std::cout << "Page references: ";
    for (uint32_t page: pageRefs)
        std::cout << page << " ";
    std::cout << "\n";

    {
        std::cout << "Optimal: \n";

        std::vector<std::vector<uint32_t>> memoryState;
        int missCount = 0;

        for (int i = 0; i < pageRefs.size(); ++i)
        {
            if (memory.Optimal(pageRefs, i))
                ++missCount;
            memoryState.push_back(memory.GetPagesInMemory());
        }
        PrintMemoryState(memoryState, capacity);

        float missRate = (float)missCount / (float)pageRefs.size();
        std::cout << std::format("miss: {0}/{1}={2}\n", missCount, pageRefs.size(), missRate);
    }
    std::cout << "\n";
    memory.Clear();

    {
        std::cout << "FIFO: \n";

        std::vector<std::vector<uint32_t>> memoryState;
        int missCount = 0;
        for (uint32_t ref : pageRefs)
        {
            if (memory.FIFO(ref))
                ++missCount;
            memoryState.push_back(memory.GetPagesInMemory());
        }
        PrintMemoryState(memoryState, capacity);

        float missRate = (float)missCount / (float)pageRefs.size();
        std::cout << std::format("miss: {0}/{1}={2}\n", missCount, pageRefs.size(), missRate);
    }
    std::cout << "\n";
    memory.Clear();


    {
        std::cout << "LRU: \n";

        std::vector<std::vector<uint32_t>> memoryState;
        int missCount = 0;
        for (uint32_t ref : pageRefs)
        {
            if (memory.LRU(ref))
                ++missCount;
            memoryState.push_back(memory.GetPagesInMemory());
        }
        PrintMemoryState(memoryState, capacity);

        float missRate = (float)missCount / (float)pageRefs.size();
        std::cout << std::format("miss: {0}/{1}={2}\n", missCount, pageRefs.size(), missRate);
    }
}