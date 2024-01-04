#include "BankersAlgorithm.h"

#include "Utils/ConsoleTextColor.h"

#include <format>
#include <iostream>

void BankerAlgorithm::AddProcess(const std::vector<int>& max, const std::vector<int>& allocation)
{
    ProcessInfo process;
    process.Max = max;
    process.Allocation = allocation;

    process.Need.resize(m_ResourceTypeCount);
    for (int i = 0; i < m_ResourceTypeCount; ++i)
        process.Need[i] = max[i] - allocation[i];

    m_Processes.push_back(process);
}

bool BankerAlgorithm::RequestResources(int processID, const std::vector<int>& request)
{
    for (int i = 0; i < m_ResourceTypeCount; ++i)
    {
        // Step1: Check if request <= need
        if (request[i] > m_Processes[processID].Need[i])
        {
            SetConsoleTextColor(ConsoleTextColor::DarkRed);
            std::cout << "\nRequest > Need!\n";
            SetConsoleTextColor(ConsoleTextColor::Gray);
            return false;
        }

        // Step2: Check if request <= available
        if (request[i] > m_Available[i])
        {
            SetConsoleTextColor(ConsoleTextColor::DarkRed);
            std::cout << "\nRequest > Available!\n";
            SetConsoleTextColor(ConsoleTextColor::Gray);
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
    SetConsoleTextColor(ConsoleTextColor::White);
    std::string separator(80, '-');
    std::cout << separator << "\n";
    std::cout << std::format("{:<7} | {:<10} | {:<10} | {:<10} | {:<18} | {}\n",
        "Process", "Work", "Need", "Allocation", "Work + Allocation", "Finished");
    std::cout << separator << "\n";

    while (!IsFinished(finish))
    {
        int index = FindSuitableProcess(finish, work);
        if (index == -1)
        {
            SetConsoleTextColor(ConsoleTextColor::DarkRed);
            std::cout << "\nAllocation failed!\n";
            SetConsoleTextColor(ConsoleTextColor::Gray);

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

    SetConsoleTextColor(ConsoleTextColor::Green);
    std::cout << "\nSucceeded!" << std::endl;
    SetConsoleTextColor(ConsoleTextColor::Gray);

    return true;
}

void BankerAlgorithm::PrintInfo() const
{
    SetConsoleTextColor(ConsoleTextColor::White);
    if (m_Processes.empty())
    {
        std::cout << "No processes.\n";
        SetConsoleTextColor(ConsoleTextColor::Gray);
        return;
    }

    std::string separator(59, '-');
    std::cout << separator << "\n";
    std::cout << std::format("{:<7} | {:<10} | {:<10} | {:<10} | {}\n",
        "Process", "Max", "Allocation", "Need", "Available");
    std::cout << separator << "\n";

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
    SetConsoleTextColor(ConsoleTextColor::Gray);
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


