#include "ProcessScheduler.h"

#include "Utils/ConsoleTextColor.h"

#include <algorithm>
#include <iostream>
#include <format>

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

static std::string SeparatorString(AlgorithmType type)
{
    switch (type)
    {
        case Priority:  return std::string(64, '-');
        case RoundRobin: return std::string(50, '-');
    }
    return "";
}

void ProcessScheduler::Reset(uint32_t processCount)
{
    m_TotalCPUTime = 0;

    m_Processes.clear();
    m_Processes.resize(processCount);

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
    std::cout << std::format("Algorithm: {}\n", AlgorithmTypeString(m_AlgorithmType));
    std::cout << std::format("Total CPU time: {}\n", m_TotalCPUTime);
    std::string separator = SeparatorString(m_AlgorithmType);
    std::cout << separator << "\n";

    std::cout << std::format("{:<12} | {:<10} | {:<10} | ", "Process ID", "CPU Time", "Need Time");
    if (m_AlgorithmType == AlgorithmType::Priority)
        std::cout << std::format("{:<10} | ", "Priority");
    std::cout << std::format("{:<9}\n", "State");
    std::cout << separator << "\n";

    for (auto& p : m_Processes)
    {
        if (p.State == ProcessState::Working)
            SetConsoleTextColor(ConsoleTextColor::Green);
        else if(p.State == ProcessState::Finished)
            SetConsoleTextColor(ConsoleTextColor::DarkRed);

        std::cout << std::format("{:<12} | {:<10} | {:<10} | ", p.ProcessID, p.CPUTime, p.NeedTime);
        if (m_AlgorithmType == AlgorithmType::Priority)
            std::cout << std::format("{:<10} | ", p.Priority);
        std::cout << std::format("{:<9}\n", StateString(p.State));

        SetConsoleTextColor(ConsoleTextColor::Gray);
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
    if (!m_WorkingProcess)
    {
        m_WorkingProcess = FetchNextProcess();
    }
    else if (m_WorkingProcess->NeedTime == 0)
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
