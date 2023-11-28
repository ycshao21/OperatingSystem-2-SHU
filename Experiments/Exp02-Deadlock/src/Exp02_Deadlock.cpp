#include <iostream>
#include <vector>
#include <conio.h>
#include <string>

struct ProcessInfo
{
    std::vector<int> Max;
    std::vector<int> Allocation;
    std::vector<int> Need;
    bool Finished = false;

    void Init(size_t resourceTypeCount)
    {
        Max.resize(resourceTypeCount);
        Allocation.resize(resourceTypeCount);
        Need.resize(resourceTypeCount);
        Finished = false;
    }
};

int s_ResourceTypeCount = -1;
std::vector<ProcessInfo> s_Processes;
std::vector<int> s_Available;

std::vector<int> s_Request;
std::vector<int> s_Work;

void Init()
{
    int processCount = -1;
    std::cout << "Please enter the number of processes: ";
    std::cin >> processCount;

    std::cout << "Please enter the number of resource types: ";
    std::cin >> s_ResourceTypeCount;

    s_Processes.resize(processCount);
    for (int i = 0; i < processCount; ++i)
    {
        auto& process = s_Processes[i];
        process.Init(s_ResourceTypeCount);
        std::cout << "P" << i << ": ";

        std::cout << "Max: ";
        for (int j = 0; j < s_ResourceTypeCount; j++)
            std::cin >> process.Max[j];

        std::cout << "Allocate: ";
        for (int j = 0; j < s_ResourceTypeCount; j++)
        {
            std::cin >> process.Allocation[j];
            process.Need[j] = process.Max[j] - process.Allocation[j];
            if (process.Need[j] < 0)
                throw;
        }
    }

    s_Available.resize(s_ResourceTypeCount);
    std::cout << "Please enter the number of available resources: ";
    for (int i = 0; i < s_ResourceTypeCount; ++i)
        std::cin >> s_Available[i];
}

bool RequestResource()
{
    int processID = 0;
    std::cout << "Please enter process ID: ";
    std::cin >> processID;
    
    auto& rp = s_Processes[processID];
    std::cout << "Please enter request: ";
    s_Request.resize(s_ResourceTypeCount);
    for (int j = 0; j < s_ResourceTypeCount; ++j)
        std::cin >> s_Request[j];

    // Step1: Check if request <= need
    for (int i = 0; i < s_ResourceTypeCount; ++i)
    {
        if (s_Request[i] > rp.Need[i])
        {
            std::cout << "request > need!" << "\n";
            return false;
        }
    }

    // Step2: Check if request <= available
    for (int i = 0; i < s_ResourceTypeCount; ++i)
    {
        if (s_Request[i] > s_Available[i])
        {
            std::cout << "request > available!" << "\n";
            return false;
        }
    }

    // Step3: Assume that the request can be satisfied
    for (int i = 0; i < s_ResourceTypeCount; ++i)
    {
        s_Available[i] -= s_Request[i];
        rp.Need[i] -= s_Request[i];
        rp.Allocation[i] += s_Request[i];
    }
    s_Work = s_Available;

    // Step4: Check security
    while (true)
    {
        // Check if all processes are finished.
        {
            bool completed = true;
            for (auto& p : s_Processes)
            {
                if (!p.Finished)
                {
                    completed = false;
                    break;
                }
            }

            if (completed)
            {
                std::cout << "Finished!" << std::endl;
                return true;
            }
        }

        // Try to find a suitable process
        int index = -1;
        for (int i = 0; i < s_Processes.size(); ++i)
        {
            if (s_Processes[i].Finished)
                continue;

            bool suitable = true;
            for (int j = 0; j < s_ResourceTypeCount; ++j)
            {
                if (s_Processes[i].Need[j] > s_Work[j])
                {
                    suitable = false;
                    break;
                }
            }
            if (suitable)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
        {
            std::cout << "No process can allocate resource!!!" << "\n";
            return false;
        }

        auto& p = s_Processes[index];
        std::cout << index << ": ";
        std::cout << "Work ";
        for (int j = 0; j < s_ResourceTypeCount; ++j)
            std::cout << s_Work[j] << " ";

        std::cout << "| Need ";
        for (int j = 0; j < s_ResourceTypeCount; ++j)
            std::cout << p.Need[j] << " ";

        std::cout << "| Allocation ";
        for (int j = 0; j < s_ResourceTypeCount; ++j)
            std::cout << p.Allocation[j] << " ";

        std::cout << "| Word + Allocation ";
        for (int j = 0; j < s_ResourceTypeCount; ++j)
        {
            s_Work[j] += p.Allocation[j];
            std::cout << s_Work[j] << " ";
        }

        std::cout << "| Finished!" << "\n";
        p.Finished = true;
    }
}

int main()
{
    while (true)
    {
        system("cls");

        Init();

        RequestResource();
        std::cout << "Press any key to return...\n";
        _getch();
    }
}