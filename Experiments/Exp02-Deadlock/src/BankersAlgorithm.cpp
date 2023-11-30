#include "BankersAlgorithm.h"

#include <format>
#include <iostream>
#include "Windows.h"

void BankerAlgorithm::AddProcess(const std::vector<int>& maxDemands, const std::vector<int>& allocatedResources)
{
    ProcessInfo process;
    process.Max = maxDemands;
    process.Allocation = allocatedResources;

    process.Need.resize(m_ResourceTypeCount);
    for (int i = 0; i < m_ResourceTypeCount; ++i)
        process.Need[i] = maxDemands[i] - allocatedResources[i];

    m_Processes.push_back(process);
}

bool BankerAlgorithm::RequestResources(int processID, const std::vector<int>& request)
{
    for (int i = 0; i < m_ResourceTypeCount; ++i)
    {
        // Step1: Check if request <= need
        if (request[i] > m_Processes[processID].Need[i])
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);  // Set color to Red
            std::cout << "\nRequest > Need!\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
            return false;
        }

        // Step2: Check if request <= available
        if (request[i] > m_Available[i])
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);  // Set color to Red
            std::cout << "\nRequest > Available!\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
            return false;
        }
    }

    // Step3: Assume that the request can be satisfied
    auto& rp = m_Processes[processID];
    for (int i = 0; i < m_ResourceTypeCount; ++i)
    {
        m_Available[i] -= request[i];
        rp.Need[i] -= request[i];
        rp.Allocation[i] += request[i];
    }

    // Step4: Safety algorithm
    std::vector<int> work(m_Available);
    std::vector<uint8_t> finish(m_Processes.size(), 0);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);  // Set color to Bright White
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << std::format("{:<7} | {:<10} | {:<10} | {:<10} | {:<18} | {}\n", "Process", "Work", "Need", "Allocation", "Work + Allocation", "Finished");
    std::cout << "--------------------------------------------------------------------------------\n";

    int index = -1;
    while (true)
    {
        if (IsFinished(finish))
            break;

        index = FindSuitableProcess(finish, work);
        if (index == -1)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);  // Set color to Red
            std::cout << "\nAllocation failed!\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color

            // Roll back
            for (int i = 0; i < m_ResourceTypeCount; ++i)
            {
                m_Available[i] += request[i];
                rp.Need[i] += request[i];
                rp.Allocation[i] -= request[i];
            }
            return false;
        }

        // Update
        finish[index] = 1;

        std::cout << std::format("{:<7} | ", index);

        {
            std::string output;
            for (int v : work)
                output += std::format("{} ", v);
            std::cout << std::format("{:<10} | ", output);
        }

        {
            std::string output;
            for (int v : m_Processes[index].Need)
                output += std::format("{} ", v);
            std::cout << std::format("{:<10} | ", output);
        }

        {
            std::string output;
            for (int v : m_Processes[index].Allocation)
                output += std::format("{} ", v);
            std::cout << std::format("{:<10} | ", output);
        }

        {
            std::string output;
            for (int j = 0; j < m_ResourceTypeCount; ++j)
            {
                work[j] += m_Processes[index].Allocation[j];
                output += std::format("{} ", work[j]);
            }
            std::cout << std::format("{:<18} | ", output);
        }

        std::cout << "true\n";
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);  // Set color to Green
    std::cout << "\nSucceeded!" << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color

    return true;
}

void BankerAlgorithm::PrintInfo() const
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);  // Set color to Bright White
    if (m_Processes.empty())
    {
        std::cout << "No processes.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
        return;
    }

    std::cout << "-----------------------------------------------------------\n";
    std::cout << std::format("{:<7} | {:<10} | {:<10} | {:<10} | {}\n", "Process", "Max", "Allocation", "Need", "Available");
    std::cout << "-----------------------------------------------------------\n";

    for (size_t i = 0; i < m_Processes.size(); ++i)
    {
        auto& p = m_Processes[i];
        std::cout << std::format("{:<7} | ", i);

        {
            std::string output;
            for (int v : p.Max)
                output += std::format("{} ", v);
            std::cout << std::format("{:<10} | ", output);
        }

        {
            std::string output;
            for (int v : p.Allocation)
                output += std::format("{} ", v);
            std::cout << std::format("{:<10} | ", output);
        }

        {
            std::string output;
            for (int v : p.Need)
                output += std::format("{} ", v);
            std::cout << std::format("{:<10} | ", output);
        }

        if (i == 0)
        {
            std::string output;
            for (int v : m_Available)
                std::cout << v << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
}

bool BankerAlgorithm::IsFinished(const std::vector<uint8_t>& finish) const
{
    for (uint8_t v: finish)
    {
        if (v == 0)
            return false;
    }
    return true;
}

int BankerAlgorithm::FindSuitableProcess(const std::vector<uint8_t>& finish, const std::vector<int>& work)
{
    for (int i = 0; i < (int)m_Processes.size(); ++i)
    {
        if (finish[i] == 1)
            continue;

        bool suitable = true;
        for (uint32_t j = 0; j < m_ResourceTypeCount; ++j)
        {
            if (m_Processes[i].Need[j] > work[j])
            {
                suitable = false;
                break;
            }
        }
        if (suitable)
            return i;
    }
    return -1;
}

void BankerAlgorithm::Reset()
{
    m_Available.clear();
    m_Processes.clear();
    m_ResourceTypeCount = 0;
}


