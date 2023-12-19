#pragma once
#include <list>
#include <vector>

class PagedMemory
{
public:
    PagedMemory() = default;
    ~PagedMemory() = default;

    void SetVirtualMemorySize(uint32_t virtualMemorySize) { m_VirtualMemorySize = virtualMemorySize; }

    uint32_t GetPageSize() const { return m_PageSize; }
    void SetPageSize(uint32_t pageSize) { m_PageSize = pageSize; }

    void Clear() { m_Pages.clear(); }

    std::vector<uint32_t> GetPagesInMemory() const { return std::vector<uint32_t>(m_Pages.begin(), m_Pages.end()); }

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
    std::list<uint32_t> m_Pages;
};
