#pragma once
#include <list>
#include <vector>

class MemoryManager
{
public:
    MemoryManager() = default;
    ~MemoryManager() = default;

    void GenerateInstructions();

    void SetPageSize(uint32_t pageSize) { m_PageSize = pageSize; }
    uint32_t GetPageSize() const { return m_PageSize; }

    void ClearPages() { m_Pages.clear(); }

    void Optimal();
    void FIFO();
    void LRU();
private:
    bool IsMemoryFull();
    uint32_t GetPageIndex(uint32_t instructionIndex);
    uint32_t GetPageToReplace();
private:
    const uint32_t m_VirtualMemorySize = 32;
    uint32_t m_PageSize = 0;
    std::list<uint32_t> m_Pages;

    const uint32_t m_InstructionCount = 320;
    std::vector<uint32_t> m_InstructionSequence;
};
