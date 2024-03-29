#include "ProcessScheduler.h"

#include "Utils/Random.h"

#include <iostream>
#include <conio.h>

int main()
{
    Random::Init();
    ProcessScheduler scheduler;

    while (true)
    {
        std::system("cls");
        std::cout << "Process Scheduling\n"
                  << "[1] Priority\n"
                  << "[2] Round Robin\n"
                  << "[3] Exit\n";

        char choice = _getch();
        if (choice < '1' || choice > '3')
            continue;
        if (choice == '3')
            break;

        scheduler.SetAlgorithmType((AlgorithmType)(choice - '0'));

        std::cout << "Please input the number of processes: ";
        uint32_t processCount;
        std::cin >> processCount;
        scheduler.Reset(processCount);

        std::system("cls");
        scheduler.PrintProcesses();

        while (!scheduler.IsDone())
        {
            _getch();
            scheduler.ScheduleByOneStep();
            scheduler.PrintProcesses();
        }

        std::cout << "Press any key to return...\n";
        _getch();
    }
}