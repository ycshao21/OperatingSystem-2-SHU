#include <iostream>
#include <vector>
#include <conio.h>

#include "Random.h"
#include "ProcessScheduler.h"

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

        std::system("cls");
        scheduler.Reset();
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