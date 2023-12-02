#include <iostream>
#include "Random.h"

#include "MemoryManager.h"

int main()
{
    Random::Init();
    MemoryManager memoryManager;
    memoryManager.GenerateInstructions();

    for (uint32_t i = 8; i >= 1; --i)
    {
        std::cout << "\nPage size: " << i << std::endl;
        memoryManager.SetPageSize(i);

        memoryManager.Optimal();
        memoryManager.ClearPages();

        memoryManager.FIFO();
        memoryManager.ClearPages();

        memoryManager.LRU();
        memoryManager.ClearPages();
    }
}