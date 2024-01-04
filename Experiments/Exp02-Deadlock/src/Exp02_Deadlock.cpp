#include "BankersAlgorithm.h"
#include "Utils/ConsoleTextColor.h"

#include <string>
#include <format>
#include <vector>
#include <iostream>
#include <conio.h>

int main()
{
    BankerAlgorithm algorithm;

    while (true)
    {
        system("cls");
        std::cout << "Banker's Algorithm\n"
                  << "[1] Initialize\n"
                  << "[2] Request Resources\n"
                  << "[3] Exit\n";

        std::cout << "\n";
        algorithm.PrintInfo();
        std::cout << "\n";

        char choice = _getch();
        if (choice < '1' || choice > '3')
            continue;
        if (choice == '3')
            break;

        switch (choice - '0')
        {
            case 1:
            {
                std::cout << "Initialize\n";
                algorithm.Reset();

                int processCount = -1;
                SetConsoleTextColor(ConsoleTextColor::Yellow);
                std::cout << "Number of processes: ";
                SetConsoleTextColor(ConsoleTextColor::Gray);
                std::cin >> processCount;

                int typeCount = -1;
                SetConsoleTextColor(ConsoleTextColor::Yellow);
                std::cout << "Number of resource types: ";
                SetConsoleTextColor(ConsoleTextColor::Gray);
                std::cin >> typeCount;

                algorithm.SetResourceTypeCount(typeCount);
                std::cout << "\n";

                std::string separator(37, '-');
                for (int i = 0; i < processCount; ++i)
                {
                    SetConsoleTextColor(ConsoleTextColor::Yellow);
                    std::cout << separator << "\n";
                    std::cout << std::format("P{}\n", i);
                    std::cout << separator << "\n";

                    std::cout << std::format("Max({}): ", typeCount);
                    SetConsoleTextColor(ConsoleTextColor::Gray);
                    std::vector<int> maxDemands(typeCount);
                    for (auto& v: maxDemands)
                        std::cin >> v;

                    SetConsoleTextColor(ConsoleTextColor::Yellow);
                    std::cout << std::format("Allocation({}): ", typeCount);
                    SetConsoleTextColor(ConsoleTextColor::Gray);
                    std::vector<int> allocation(typeCount);
                    for (auto& v: allocation)
                        std::cin >> v;

                    algorithm.AddProcess(maxDemands, allocation);
                }

                SetConsoleTextColor(ConsoleTextColor::Yellow);
                std::cout << std::format("\nAvailable({}): ", typeCount);
                SetConsoleTextColor(ConsoleTextColor::Gray);
                std::vector<int> available(typeCount);
                for (auto& v: available)
                    std::cin >> v;
                algorithm.SetAvailable(available);

                SetConsoleTextColor(ConsoleTextColor::Green);
                std::cout << "\nInitialized!\n";
                SetConsoleTextColor(ConsoleTextColor::Gray);
                break;
            }
            case 2:
            {
                std::cout << "Request Resources\n";
                int processID = 0;
                SetConsoleTextColor(ConsoleTextColor::Yellow);
                std::cout << "Process ID: ";
                SetConsoleTextColor(ConsoleTextColor::Gray);
                std::cin >> processID;
                
                std::vector<int> request(algorithm.GetResourceTypeCount());
                SetConsoleTextColor(ConsoleTextColor::Yellow);
                std::cout << std::format("Request({}): ", algorithm.GetResourceTypeCount());
                SetConsoleTextColor(ConsoleTextColor::Gray);
                for (auto& v: request)
                    std::cin >> v;

                algorithm.RequestResources(processID, request);
                break;
            }
        }

        std::cout << "Press any key to return...\n";
        _getch();
    }
}