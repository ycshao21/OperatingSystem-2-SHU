#include <iostream>
#include <string>
#include <vector>
#include <format>
#include "FileControlBlock.h"
#include "Utils/ConsoleTextColor.h"

static std::vector<std::string> s_ValidCommands{
    "create", "delete",
    "open", "close",
    "write", "read", "execute",
    "bye"
};

struct UserInfo
{
    std::string Username;
    std::string Password;
    FileControlBlock FCB;

    UserInfo(const std::string& username, const std::string& password)
        : Username(username), Password(password), FCB()
    {
    }
};

int main()
{
    std::vector<UserInfo> users{
        UserInfo("Matthew", "114514"),
        UserInfo("user1", "666")
    };

    while (true)
    {
        system("cls");
        std::cout << "Login\n";

        std::string username, password;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

        auto userIter = std::find_if(users.begin(), users.end(),
            [&username, &password](const UserInfo& user)
            {
                return user.Username == username && user.Password == password;
            }
        );
        if (userIter == users.end())
        {
            SetConsoleTextColor(ConsoleTextColor::DarkRed);
            std::cout << "Username or password is incorrect!\n";
            SetConsoleTextColor(ConsoleTextColor::Gray);
            system("pause");
            continue;
        }

        FileControlBlock& fcb = userIter->FCB;
        while (true)
        {
            system("cls");
            fcb.DisplayInfo();

            SetConsoleTextColor(ConsoleTextColor::Yellow);
            std::cout << "Commands\n";
            std::cout << std::string(15, '-') << "\n";
            for (int i = 0; i < (int)s_ValidCommands.size(); ++i)
                std::cout << ">> " << s_ValidCommands[i] << "\n";
            std::cout << std::string(15, '-') << "\n";
            SetConsoleTextColor(ConsoleTextColor::Gray);

            std::cout << "Your command: ";
            std::string command;
            std::cin >> command;

            if (command == "create")
                fcb.Create();
            else if (command == "delete")
                fcb.Delete();
            else if (command == "open")
                fcb.Open();
            else if (command == "close")
                fcb.Close();
            else if (command == "write")
                fcb.Write();
            else if (command == "read")
                fcb.Read();
            else if (command == "execute")
                fcb.Execute();
            else if (command == "bye")
            {
                if (fcb.Bye())
                {
                    system("pause");
                    break;
                }
            }
            else
            {
                SetConsoleTextColor(ConsoleTextColor::DarkRed);
                std::cout << "Unknown command!\n";
                SetConsoleTextColor(ConsoleTextColor::Gray);
            }
            system("pause");
        }
    }
}