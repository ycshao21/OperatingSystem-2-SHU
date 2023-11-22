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
        std::cout << "Process Scheduling" << "\n"
                  << "[1] Priority"       << "\n"
                  << "[2] Round Robin"    << "\n"
                  << "[3] Exit"           << "\n";

        int choice = _getch() - '0';
        if (choice == 3) break;
        if (choice > 3) continue;

        scheduler.SetAlgorithmType((AlgorithmType)choice);

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