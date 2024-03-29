#pragma once
#include "PCB.h"

#include <vector>
#include <list>

enum AlgorithmType : uint8_t
{
    Priority = 1,
    RoundRobin = 2
};

class ProcessScheduler
{
public:
    bool IsDone();

    void Reset(uint32_t processCount);
    void SetAlgorithmType(AlgorithmType type) { m_AlgorithmType = type; }
    void PrintProcesses();

    void ScheduleByOneStep();
private:
    void PriorityScheduleByOneStep();
    void RoundRobinScheduleByOneStep();

    PCB* FetchNextProcess();
    void SortProcessesByPriority();
private:
    PCB* m_WorkingProcess = nullptr;
    std::list<PCB*> m_ReadyProcesses;

    std::vector<PCB> m_Processes;
    uint32_t m_TotalCPUTime = 0;
    AlgorithmType m_AlgorithmType = AlgorithmType::Priority;
};
