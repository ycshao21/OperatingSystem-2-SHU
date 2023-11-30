#pragma once
#include <vector>

class BankerAlgorithm
{
    struct ProcessInfo
    {
        std::vector<int> Max;
        std::vector<int> Allocation;
        std::vector<int> Need;
    };
public:
    void SetResourceTypeCount(uint32_t count) { m_ResourceTypeCount = count; }
    uint32_t GetResourceTypeCount() const { return m_ResourceTypeCount; }

    void Reset();
    bool HasProcesses() const { return !m_Processes.empty(); }

    void AddProcess(const std::vector<int>& max, const std::vector<int>& allocation);
    void SetAvailable(const std::vector<int>& available) { m_Available = available; }
    bool RequestResources(int processID, const std::vector<int>& request);

    void PrintInfo() const;
private:
    int FindSuitableProcess(const std::vector<uint8_t>& finish, const std::vector<int>& work);
    bool IsFinished(const std::vector<uint8_t>& finish) const;
private:
    uint32_t m_ResourceTypeCount = 0;
    std::vector<ProcessInfo> m_Processes;
    std::vector<int> m_Available;
};
