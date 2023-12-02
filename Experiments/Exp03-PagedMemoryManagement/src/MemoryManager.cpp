#include "MemoryManager.h"
#include "Random.h"

#include <iostream>

void MemoryManager::GenerateInstructions()
{
    // Generate instruction sequence
    m_InstructionSequence.resize(m_InstructionCount);

    uint32_t m = Random::GenerateInt32(0, m_InstructionCount - 2);
    for (int i = 0; i < m_InstructionCount; i += 4)
    {
        m_InstructionSequence[i] = m;

        m_InstructionSequence[i + 1] = m + 1;
        do
        {
            m = Random::GenerateInt32(0, m + 1);
        } while (m > m_InstructionCount - 4);

        m_InstructionSequence[i + 2] = m;
        m_InstructionSequence[i + 3] = m + 1;
        m = Random::GenerateInt32(m + 2, m_InstructionCount - 2);
    }

    // Print instruction sequence
    for (int i = 0; i < m_InstructionCount; ++i)
    {
        std::cout << "[" << i << "]: " << m_InstructionSequence[i] << " ";
        if ((i + 1) % 10 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool MemoryManager::IsMemoryFull()
{
    return m_Pages.size() == m_VirtualMemorySize / m_PageSize;
}

uint32_t MemoryManager::GetPageIndex(uint32_t instructionIndex)
{
    return instructionIndex / 10;
}

uint32_t MemoryManager::GetPageToReplace()
{
    std::list<uint32_t> copiedPages(m_Pages.begin(), m_Pages.end());
    for (uint32_t i = 0; i < m_InstructionCount; ++i)
    {
        copiedPages.remove(GetPageIndex(m_InstructionSequence[i]));
        if (copiedPages.size() == 1)
            break;
    }
    return copiedPages.front();
}

void MemoryManager::Optimal()
{
    uint32_t missCount = 0;
    for (uint32_t instructionIndex: m_InstructionSequence)
    {
        uint32_t pageIndex = GetPageIndex(instructionIndex);
        auto iter = std::find(m_Pages.begin(), m_Pages.end(), pageIndex);
        if (iter == m_Pages.end())  // Page is not in memory
        {
            ++missCount;

            if (IsMemoryFull())
            {
                uint32_t pageToReplace = GetPageToReplace();
                m_Pages.remove(pageToReplace);
            }
            m_Pages.push_back(pageIndex);
        }
    }

    float hitRate = 1.0f - (float)missCount / m_InstructionCount;
    std::cout << "Optimal: " << hitRate << std::endl;
}

void MemoryManager::FIFO()
{
    uint32_t missCount = 0;
    for (uint32_t instructionIndex: m_InstructionSequence)
    {
        uint32_t pageIndex = GetPageIndex(instructionIndex);
        auto iter = std::find(m_Pages.begin(), m_Pages.end(), pageIndex);
        if (iter == m_Pages.end())  // Page is not in memory
        {
            ++missCount;

            if (IsMemoryFull())
                m_Pages.pop_front();
            m_Pages.push_back(pageIndex);
        }
    }

    float hitRate = 1.0f - (float)missCount / m_InstructionCount;
    std::cout << "FIFO: " << hitRate << std::endl;
}

void MemoryManager::LRU()
{
    uint32_t missCount = 0;
    for (uint32_t instructionIndex: m_InstructionSequence)
    {
        uint32_t pageIndex = GetPageIndex(instructionIndex);
        auto iter = std::find(m_Pages.begin(), m_Pages.end(), pageIndex);
        if (iter == m_Pages.end())  // Page is not in memory
        {
            ++missCount;

            if (IsMemoryFull())
                m_Pages.pop_front();
        }
        else
        {
            m_Pages.erase(iter);
        }
        m_Pages.push_back(pageIndex);
    }

    float hitRate = 1.0f - (float)missCount / m_InstructionCount;
    std::cout << "LRU: " << hitRate << std::endl;
}

