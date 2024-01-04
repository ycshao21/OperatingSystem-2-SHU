#pragma once
#include <list>
#include <vector>

class PagedMemory
{
public:
    PagedMemory() = default;
    ~PagedMemory() = default;

    const std::vector<uint32_t>& GetPages() const { return m_Pages; }

    void Clear();
    void SetVirtualMemorySize(uint32_t virtualMemorySize) { m_VirtualMemorySize = virtualMemorySize; }

    void SetPageSize(uint32_t pageSize) { m_PageSize = pageSize; }
    uint32_t GetPageSize() const { return m_PageSize; }

    uint32_t GetMaxPageCount() const { return m_VirtualMemorySize / m_PageSize; }

    void SetPhysicalBlockCount(uint32_t physicalBlockCount) { m_PhysicalBlockCount = physicalBlockCount; }
    uint32_t GetPhysicalBlockCount() const { return m_PhysicalBlockCount; }

    // Algorithms for page replacement, return whether the page referenced is missing.

    bool Optimal(const std::vector<uint32_t>& pageRefs, int refIndex);
    bool FIFO(uint32_t ref);
    bool LRU(uint32_t ref);
private:
    bool IsMemoryFull();
    uint32_t GetOptimalPageToReplace(const std::vector<uint32_t>& pageRefs, int refIndex) const;
private:
    uint32_t m_VirtualMemorySize = 32;
    uint32_t m_PageSize = 1;
    std::vector<uint32_t> m_Pages;
    std::list<uint32_t*> m_ReplaceOrder;
    uint32_t m_PhysicalBlockCount = 4;
};
