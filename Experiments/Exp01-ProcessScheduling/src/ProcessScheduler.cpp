#include "ProcessScheduler.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

static std::string StateString(ProcessState state)
{
    switch (state)
    {
        case ProcessState::Ready:    return "Ready";
        case ProcessState::Working:  return "Working";
        case ProcessState::Finished: return "Finished";
    }
    return "";
}

static std::string AlgorithmTypeString(AlgorithmType type)
{
    switch (type)
    {
        case Priority:   return "Priority";
        case RoundRobin: return "Round Robin";
    }
    return "";
}

void ProcessScheduler::Reset()
{
    m_TotalCPUTime = 0;

    m_Processes.clear();
    m_Processes.resize(5);

    m_WorkingProcess = nullptr;
    for (int i = 0; i < m_Processes.size(); ++i)
        m_ReadyProcesses.push_back(&m_Processes[i]);

    if (m_AlgorithmType == AlgorithmType::Priority)
        SortProcessesByPriority();
}

bool ProcessScheduler::IsDone()
{
    return m_ReadyProcesses.empty() && !m_WorkingProcess;
}

void ProcessScheduler::PrintProcesses()
{
    std::cout << "Algorithm: " << AlgorithmTypeString(m_AlgorithmType) << "\n";
    std::cout << "Total CPU time: " << m_TotalCPUTime << "\n";
    std::cout << "-----------------------------------------------------------------\n";

    std::cout << std::setw(12) << "Process ID" << " | "
              << std::setw(10) << "CPU Time"   << " | "
              << std::setw(10) << "Need Time"  << " | ";
    if (m_AlgorithmType == AlgorithmType::Priority)
        std::cout << std::setw(10) << "Priority" << " | ";
    std::cout << std::setw(10)  << "State" << "\n";

    std::cout << "-----------------------------------------------------------------\n";

    for (auto& p : m_Processes)
    {
        if (p.State == ProcessState::Working)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);  // Set color to GREEN
        else if(p.State == ProcessState::Finished)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);  // Set color to RED
        
        std::cout << std::setw(12) << p.ProcessID << " | "
                  << std::setw(10) << p.CPUTime   << " | "
                  << std::setw(10) << p.NeedTime  << " | ";
        if (m_AlgorithmType == AlgorithmType::Priority)
            std::cout << std::setw(10) << p.Priority << " | ";
        std::cout << std::setw(10)  << StateString(p.State) << "\n";

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
    }
    std::cout << "\n";

    if (IsDone())
        std::cout << "Scheduling finished!\n";
}

void ProcessScheduler::ScheduleByOneStep()
{
    ++m_TotalCPUTime;

    switch (m_AlgorithmType)
    {
        case AlgorithmType::Priority:   { PriorityScheduleByOneStep();   break; }
        case AlgorithmType::RoundRobin: { RoundRobinScheduleByOneStep(); break; }
    }
}

void ProcessScheduler::PriorityScheduleByOneStep()
{
    if (m_WorkingProcess)
    {
        if (m_WorkingProcess->NeedTime == 0)
        {
            m_WorkingProcess->State = ProcessState::Finished;
            m_WorkingProcess = FetchNextProcess();
        }
        else if (!m_ReadyProcesses.empty() && m_WorkingProcess->Priority < m_ReadyProcesses.front()->Priority)
        {
            m_WorkingProcess->State = ProcessState::Ready;
            m_ReadyProcesses.push_back(m_WorkingProcess);
            SortProcessesByPriority();
            m_WorkingProcess = FetchNextProcess();
        }
    }
    else
    {
        m_WorkingProcess = FetchNextProcess();
    }

    if (m_WorkingProcess)
    {
        ++m_WorkingProcess->CPUTime;
        --m_WorkingProcess->NeedTime;
        m_WorkingProcess->Priority -= 3;
        m_WorkingProcess->State = ProcessState::Working;

        if (m_ReadyProcesses.empty() && m_WorkingProcess->NeedTime == 0)
            m_WorkingProcess = nullptr;
    }
}

void ProcessScheduler::RoundRobinScheduleByOneStep()
{
    if (m_WorkingProcess)
    {
        if (m_WorkingProcess->NeedTime == 0)
        {
            m_WorkingProcess->State = ProcessState::Finished;
        }
        else
        {
            m_WorkingProcess->State = ProcessState::Ready;
            m_ReadyProcesses.push_back(m_WorkingProcess);
        }
    }

    m_WorkingProcess = FetchNextProcess();

    if (m_WorkingProcess)
    {
        ++m_WorkingProcess->CPUTime;
        --m_WorkingProcess->NeedTime;
        m_WorkingProcess->State = ProcessState::Working;

        if (m_ReadyProcesses.empty() && m_WorkingProcess->NeedTime == 0)
            m_WorkingProcess = nullptr;
    }
}

PCB* ProcessScheduler::FetchNextProcess()
{
    if (m_ReadyProcesses.empty())
        return nullptr;

    auto nextProcess = m_ReadyProcesses.front();
    m_ReadyProcesses.pop_front();
    return nextProcess;
}

void ProcessScheduler::SortProcessesByPriority()
{
    m_ReadyProcesses.sort([](PCB* p1, PCB* p2) { return p1->Priority > p2->Priority; });
}
