#include <string>
#include <format>

#include <iostream>
#include <Windows.h>
#include <conio.h>

#include <vector>
#include "BankersAlgorithm.h"

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
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);  // Set color to Light Yellow
                std::cout << "Number of processes: ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
                std::cin >> processCount;

                int resourceTypeCount = -1;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);  // Set color to Light Yellow
                std::cout << "Number of resource types: ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
                std::cin >> resourceTypeCount;

                algorithm.SetResourceTypeCount(resourceTypeCount);

                for (int i = 0; i < processCount; ++i)
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);  // Set color to Light Yellow
                    std::cout << "\n-------------------------------------\n";
                    std::cout << std::format("P{}\n", i);
                    std::cout << "-------------------------------------\n";

                    std::cout << std::format("Max({}): ", resourceTypeCount);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
                    std::vector<int> maxDemands(resourceTypeCount);
                    for (auto& v: maxDemands)
                        std::cin >> v;

                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);  // Set color to Light Yellow
                    std::cout << std::format("Allocation({}): ", resourceTypeCount);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
                    std::vector<int> allocatedResources(resourceTypeCount);
                    for (auto& v: allocatedResources)
                        std::cin >> v;

                    algorithm.AddProcess(maxDemands, allocatedResources);
                }

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);  // Set color to Light Yellow
                std::cout << std::format("\nAvailable({}): ", resourceTypeCount);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
                std::vector<int> availableResources(resourceTypeCount);
                for (auto& v: availableResources)
                    std::cin >> v;
                algorithm.SetAvailable(availableResources);

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);  // Set color to Green
                std::cout << "\nInitialized!\n";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
                break;
            }
            case 2:
            {
                std::cout << "Request Resources\n";
                int processID = 0;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);  // Set color to Light Yellow
                std::cout << "Process ID: ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
                std::cin >> processID;
                
                std::vector<int> request(algorithm.GetResourceTypeCount());
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);  // Set color to Light Yellow
                std::cout << std::format("Request({}): ", algorithm.GetResourceTypeCount());
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  // Reset color
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