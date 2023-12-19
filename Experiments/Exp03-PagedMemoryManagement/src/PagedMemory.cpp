#include "PagedMemory.h"
#include "Random.h"


#include <iostream>

bool PagedMemory::IsMemoryFull()
{
    return m_Pages.size() == m_VirtualMemorySize / m_PageSize;
}

uint32_t PagedMemory::GetOptimalPageToReplace(const std::vector<uint32_t>& pageRefs, int refIndex) const
{
    std::list<uint32_t> pages(m_Pages.begin(), m_Pages.end());
    for (int i = refIndex + 1; i < pageRefs.size(); ++i)
    {
        pages.remove(pageRefs[i]);
        if (pages.size() == 1)
            break;
    }
    return pages.front();
}

bool PagedMemory::Optimal(const std::vector<uint32_t>& pageRefs, int refIndex)
{
    uint32_t ref = pageRefs[refIndex];
    bool miss = std::find(m_Pages.begin(), m_Pages.end(), ref) == m_Pages.end();
    if (miss)
    {
        if (IsMemoryFull())
            m_Pages.remove(GetOptimalPageToReplace(pageRefs, refIndex));
        m_Pages.push_back(ref);
    }
    return miss;
}

bool PagedMemory::FIFO(uint32_t ref)
{
    bool miss = std::find(m_Pages.begin(), m_Pages.end(), ref) == m_Pages.end();
    if (miss)
    {
        if (IsMemoryFull())
            m_Pages.pop_front();
        m_Pages.push_back(ref);
    }
    return miss;
}

bool PagedMemory::LRU(uint32_t ref)
{
    auto iter = std::find(m_Pages.begin(), m_Pages.end(), ref);
    bool miss = iter == m_Pages.end();
    if (miss)
    {
        if (IsMemoryFull())
            m_Pages.pop_front();
    }
    else
    {
        m_Pages.erase(iter);
    }
    m_Pages.push_back(ref);
    return miss;
}

