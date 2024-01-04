#include "PagedMemory.h"

#include "Utils/Random.h"

#include <iostream>

bool PagedMemory::IsMemoryFull()
{
    return m_Pages.size() == m_PhysicalBlockCount;
}

uint32_t PagedMemory::GetOptimalPageToReplace(
    const std::vector<uint32_t>& pageRefs,
    int refIndex) const
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

void PagedMemory::Clear()
{
    m_Pages.clear();
    m_ReplaceOrder.clear();
}

bool PagedMemory::Optimal(const std::vector<uint32_t>& pageRefs, int refIndex)
{
    uint32_t ref = pageRefs[refIndex];
    bool miss = std::find(m_Pages.begin(), m_Pages.end(), ref) == m_Pages.end();
    if (miss)
    {
        if (!IsMemoryFull())
        {
            m_Pages.push_back(ref);
        }
        else
        {
            uint32_t pageToReplace = GetOptimalPageToReplace(pageRefs, refIndex);
            auto iter = std::find(m_Pages.begin(), m_Pages.end(), pageToReplace);
            *iter = ref;
        }
    }
    return miss;
}

bool PagedMemory::FIFO(uint32_t ref)
{
    bool miss = std::find(m_Pages.begin(), m_Pages.end(), ref) == m_Pages.end();
    if (miss)
    {
        if (!IsMemoryFull())
        {
            m_Pages.push_back(ref);
            m_ReplaceOrder.push_front(&m_Pages.back());
        }
        else
        {
            uint32_t* pageToReplace = m_ReplaceOrder.back();
            m_ReplaceOrder.pop_back();
            *pageToReplace = ref;
            m_ReplaceOrder.push_front(pageToReplace);
        }
    }
    return miss;
}

bool PagedMemory::LRU(uint32_t ref)
{
    bool miss = std::find(m_Pages.begin(), m_Pages.end(), ref) == m_Pages.end();
    if (miss)
    {
        if (!IsMemoryFull())
        {
            m_Pages.push_back(ref);
            m_ReplaceOrder.push_front(&m_Pages.back());
        }
        else
        {
            uint32_t* pageToReplace = m_ReplaceOrder.back();
            m_ReplaceOrder.pop_back();
            *pageToReplace = ref;
            m_ReplaceOrder.push_front(pageToReplace);
        }
    }
    else
    {
        auto iter = std::find(m_Pages.begin(), m_Pages.end(), ref);
        m_ReplaceOrder.remove(&*iter);
        m_ReplaceOrder.push_front(&*iter);
    }
    return miss;
}

