#pragma once
#include "Random.h"
#include <numeric>

enum class ProcessState : uint8_t
{
    Ready = 0, Working, Finished
};

struct PCB
{
    uint32_t ProcessID;
    uint32_t NeedTime;
    int      Priority;
    uint32_t CPUTime = 0;
    ProcessState State = ProcessState::Ready;

    PCB()
    {
        ProcessID = (uint32_t)Random::GenerateInt32();
        NeedTime = (uint32_t)Random::GenerateInt32(1, 15);
        Priority = Random::GenerateInt32(0, 30);
    }
};

