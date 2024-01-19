#include "PagedMemory.h"

#include "Utils/Random.h"
#include "Utils/ConsoleTextColor.h"

#include <iostream>
#include <format>
#include <functional>
#include <conio.h>

std::vector<uint32_t> GeneratePageReferences(uint32_t refCount, int pageCount)
{
    std::vector<uint32_t> pageRefs(refCount);
    for (int i = 0; i < refCount; ++i)
        pageRefs[i] = Random::GenerateInt32(0, pageCount);
    return pageRefs;
}

void PrintMemorySequence(
    const std::vector<uint32_t>& pageRefs,
    const std::vector<uint8_t>& missFlags,
    const std::vector<std::vector<uint32_t>>& memoryState,
    int blockCount)
{
    std::cout << std::string(75, '-') << "\n";
    std::cout << "References | ";
    for (uint32_t page: pageRefs)
        std::cout << std::format("{:>2} ", page);
    std::cout << "\n";
    std::cout << std::string(75, '-') << "\n";
    for (int i = 0; i < blockCount; ++i)
    {
        std::cout << "   Block " << i + 1 << " | ";
        for (int j = 0; j < memoryState.size(); ++j)
        {
            if (i >= memoryState[j].size())  // Not used
                std::cout << " _ ";
            else if (memoryState[j][i] == (uint32_t)-1)  // Hit
                std::cout << " / ";
            else  // Missed
                std::cout << std::format("{:>2} ", memoryState[j][i]);
        }
        std::cout << "\n";
    }
    std::cout << std::string(75, '-') << "\n";
    std::cout << "   Missed? | ";
    int missCount = 0;
    for (uint8_t miss : missFlags)
    {
        if (miss == 1)
        {
            ++missCount;
            std::cout << " Y ";
        }
        else
        {
            std::cout << " N ";
        }
    }
    std::cout << "\n";
    std::cout << std::string(75, '-') << "\n";
    float missRate = (float)missCount / (float)pageRefs.size();
    std::cout << std::format("Miss rate: {0}/{1}={2}\n", missCount, pageRefs.size(), missRate);
}

void ExecuteAlgorithm_Task01(
    const std::string& algorithm,
    PagedMemory& memory,
    const std::vector<uint32_t>& pageRefs
)
{
    std::vector<std::vector<uint32_t>> memorySequence;
    std::vector<uint8_t> missFlags;

    SetConsoleTextColor(ConsoleTextColor::Cyan);
    std::cout << algorithm << "\n";
    SetConsoleTextColor(ConsoleTextColor::Gray);
    for (int i = 0; i < pageRefs.size(); ++i)
    {
        if (algorithm == "Optimal" && memory.Optimal(pageRefs, i)
            || algorithm == "FIFO" && memory.FIFO(pageRefs[i])
            || algorithm == "LRU" && memory.LRU(pageRefs[i]))
        {
            missFlags.push_back(1);
            memorySequence.push_back(memory.GetPages());
        }
        else
        {
            missFlags.push_back(0);
            memorySequence.push_back(std::vector<uint32_t>(memory.GetPhysicalBlockCount(), (uint32_t)-1));
        }
    }
    PrintMemorySequence(pageRefs, missFlags, memorySequence, memory.GetPhysicalBlockCount());
    memory.Clear();
}

void AlgorithmDemonstration()
{
    PagedMemory memory;

    // Memory size.
    SetConsoleTextColor(ConsoleTextColor::Yellow);
    std::cout << "Please enter the size of virtual memory: ";
    SetConsoleTextColor(ConsoleTextColor::Gray);

    uint32_t memorySize = 0;
    std::cin >> memorySize;
    memory.SetVirtualMemorySize(memorySize);

    // Page size.
    SetConsoleTextColor(ConsoleTextColor::Yellow);
    std::cout << "Please enter the size of each page: ";
    SetConsoleTextColor(ConsoleTextColor::Gray);

    uint32_t pageSize = 0;
    std::cin >> pageSize;
    memory.SetPageSize(pageSize);

    // Physical block count.
    uint32_t blockCount = 0;
    while (true)
    {
        SetConsoleTextColor(ConsoleTextColor::Yellow);
        std::cout << "Please enter the number of physical blocks: ";
        SetConsoleTextColor(ConsoleTextColor::Gray);
        std::cin >> blockCount;
        if (blockCount >= 1 && blockCount <= memory.GetMaxPageCount())
            break;
        SetConsoleTextColor(ConsoleTextColor::DarkRed);
        std::cout << "The number of physical blocks must be within [1, " << memory.GetMaxPageCount() << "]!\n";
    }
    memory.SetPhysicalBlockCount(blockCount);
    std::cout << "\n";

    // Generate page references.
    auto pageRefs = GeneratePageReferences(20, memory.GetMaxPageCount());

    // Execute algorithms.
    ExecuteAlgorithm_Task01("Optimal", memory, pageRefs);
    std::cout << "\n";
    ExecuteAlgorithm_Task01("FIFO", memory, pageRefs);
    std::cout << "\n";
    ExecuteAlgorithm_Task01("LRU", memory, pageRefs);
    std::cout << "\n";
}

void ExecuteAlgorithm_Task02(
    const std::string& algorithm,
    PagedMemory& memory,
    const std::vector<uint32_t>& pageRefs
)
{
    int missCount = 0;
    for (int i = 0; i < pageRefs.size(); ++i)
    {
        if (algorithm == "Optimal" && memory.Optimal(pageRefs, i)
            || algorithm == "FIFO" && memory.FIFO(pageRefs[i])
            || algorithm == "LRU" && memory.LRU(pageRefs[i]))
        {
            ++missCount;
        }
    }

    float hitRate = 1.0f - (float)missCount / (float)pageRefs.size();
    std::cout << std::format("{:>8} | Miss: {}  Hit rate: {}\n", algorithm, missCount, hitRate);
    memory.Clear();
}

void PerformanceTest()
{
    PagedMemory memory;

    uint32_t memorySize = 32, pageSize = 1;
    memory.SetVirtualMemorySize(memorySize);
    memory.SetPageSize(pageSize);
    SetConsoleTextColor(ConsoleTextColor::Cyan);
    std::cout << "Virtual Memory: " << memorySize << "\n";
    std::cout << "Page Size: " << pageSize << "\n";

    uint32_t refCount = 100;
    std::cout << "References: " << refCount << "\n\n";
    SetConsoleTextColor(ConsoleTextColor::Gray);

    // Generate page references.
    auto pageRefs = GeneratePageReferences(refCount, memory.GetMaxPageCount());

    std::string separator = std::string(55, '-');
    for (int blockCount = 2; blockCount <= 12; blockCount += 2)
    {
        memory.SetPhysicalBlockCount(blockCount);
        std::cout << "Physical Blocks: " << blockCount << "\n";
        std::cout << separator << "\n";
        ExecuteAlgorithm_Task02("Optimal", memory, pageRefs);
        ExecuteAlgorithm_Task02("FIFO", memory, pageRefs);
        ExecuteAlgorithm_Task02("LRU", memory, pageRefs);
        std::cout << separator << "\n\n";
    }
}

int main()
{
    Random::Init();

    while (true)
    {
        system("cls");
        std::cout << "Paged Memory Management\n"
                  << "[1] Algorithm Demonstration\n"
                  << "[2] Performance Test\n"
                  << "[3] Exit\n";
        std::cout << "\n";

        char choice = _getch();
        if (choice < '1' || choice > '3')
            continue;
        if (choice == '3')
            break;

        switch (choice - '0')
        {
            case 1: { AlgorithmDemonstration(); break; }
            case 2: { PerformanceTest(); break; }
        }
        std::cout << "Press any key to return...\n";
        _getch();
    }
}