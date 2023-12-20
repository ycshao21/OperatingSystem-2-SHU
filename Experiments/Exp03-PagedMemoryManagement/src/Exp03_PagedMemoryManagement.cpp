#include <iostream>
#include <format>
#include <functional>
#include "Random.h"
#include "PagedMemory.h"

enum class AlgorithmType: uint8_t
{
    Optimal = 0, FIFO, LRU
};

std::vector<uint32_t> GeneratePageReferences(uint32_t refCount, int pageCount)
{
    std::vector<uint32_t> pageRefs(refCount);
    for (int i = 0; i < refCount; ++i)
        pageRefs[i] = Random::GenerateInt32(0, pageCount);
    return pageRefs;
}

void PrintMemorySequence(const std::vector<std::vector<uint32_t>>& memoryState, int capacity)
{
    std::cout << "Memory Sequence: \n";
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

void Task01()
{
    // Initialize memory.
    PagedMemory memory;
    uint32_t memorySize = 3, pageSize = 1;
    memory.SetVirtualMemorySize(memorySize);
    memory.SetPageSize(pageSize);
    uint32_t capacity = memorySize / pageSize;

    // Generate page references.
    auto pageRefs = GeneratePageReferences(20, 32);
    std::cout << "Page references:\n";
    for (uint32_t page: pageRefs)
        std::cout << page << " ";
    std::cout << "\n\n";
    
    {
        std::cout << "-----------\n";
        std::cout << "| Optimal |\n";
        std::cout << "-----------\n";

        std::vector<std::vector<uint32_t>> memorySequence;
        int missCount = 0;
        for (int i = 0; i < pageRefs.size(); ++i)
        {
            if (memory.Optimal(pageRefs, i))
                ++missCount;
            memorySequence.push_back(memory.GetPagesInMemory());
        }
        PrintMemorySequence(memorySequence, capacity);

        float missRate = (float)missCount / (float)pageRefs.size();
        std::cout << std::format("miss: {0}/{1}={2}\n", missCount, pageRefs.size(), missRate);
    }
    std::cout << "\n";
    memory.Clear();

    {
        std::cout << "--------\n";
        std::cout << "| FIFO |\n";
        std::cout << "--------\n";

        std::vector<std::vector<uint32_t>> memorySequence;
        int missCount = 0;
        for (uint32_t ref : pageRefs)
        {
            if (memory.FIFO(ref))
                ++missCount;
            memorySequence.push_back(memory.GetPagesInMemory());
        }
        PrintMemorySequence(memorySequence, capacity);

        float missRate = (float)missCount / (float)pageRefs.size();
        std::cout << std::format("miss: {0}/{1}={2}\n", missCount, pageRefs.size(), missRate);
    }
    std::cout << "\n";
    memory.Clear();

    {
        std::cout << "-------\n";
        std::cout << "| LRU |\n";
        std::cout << "-------\n";

        std::vector<std::vector<uint32_t>> memorySequence;
        int missCount = 0;
        for (uint32_t ref : pageRefs)
        {
            if (memory.LRU(ref))
                ++missCount;
            memorySequence.push_back(memory.GetPagesInMemory());
        }
        PrintMemorySequence(memorySequence, capacity);

        float missRate = (float)missCount / (float)pageRefs.size();
        std::cout << std::format("miss: {0}/{1}={2}\n", missCount, pageRefs.size(), missRate);
    }
}

void Task02()
{
    // Initialize memory.
    PagedMemory memory;
    uint32_t pageSize = 1;
    memory.SetPageSize(pageSize);

    // Generate page references.
    auto pageRefs = GeneratePageReferences(200, 32);

    for (int memorySize = 2; memorySize <= 32; memorySize += 2)
    {
        memory.SetVirtualMemorySize(memorySize);
        std::cout << std::format("Memory size: {}\n", memorySize);

        {
            int missCount = 0;
            for (int i = 0; i < pageRefs.size(); ++i)
            {
                if (memory.Optimal(pageRefs, i))
                    ++missCount;
            }
            float hitRate = 1.0f - (float)missCount / (float)pageRefs.size();
            std::cout << std::format("Optimal  Miss: {0}  Hit rate: {1}\n", missCount, hitRate);
            memory.Clear();
        }

        {
            int missCount = 0;
            for (uint32_t ref: pageRefs)
            {
                if (memory.FIFO(ref))
                    ++missCount;
            }
            float hitRate = 1.0f - (float)missCount / (float)pageRefs.size();
            std::cout << std::format("FIFO     Miss: {0}  Hit rate: {1}\n", missCount, hitRate);
            memory.Clear();
        }

        {
            int missCount = 0;
            for (uint32_t ref: pageRefs)
            {
                if (memory.LRU(ref))
                    ++missCount;
            }
            float hitRate = 1.0f - (float)missCount / (float)pageRefs.size();
            std::cout << std::format("LRU      Miss: {0}  Hit rate: {1}\n", missCount, hitRate);
            memory.Clear();
        }
        std::cout << "---------------------------------------------------------------------\n";
    }
}

int main()
{
    Random::Init();
    Task02();
}