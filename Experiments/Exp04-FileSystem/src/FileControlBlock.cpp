#include "FileControlBlock.h"
#include "Utils/ConsoleTextColor.h"
#include <iostream>
#include <format>

static void PrintError(const std::string& message)
{
    SetConsoleTextColor(ConsoleTextColor::DarkRed);
    std::cout << message << "\n";
    SetConsoleTextColor(ConsoleTextColor::Gray);
}

static void PrintSuccess(const std::string& message)
{
    SetConsoleTextColor(ConsoleTextColor::Green);
    std::cout << message << "\n";
    SetConsoleTextColor(ConsoleTextColor::Gray);
}

void FileControlBlock::Create()
{
    if (m_OpenedFile)
    {
        PrintError("A file is already opened!");
        return;
    }
    if (m_Files.size() >= 10)
    {
        PrintError("The number of files should be less than 10!");
        return;
    }

    std::cout << "Filename (less than 9 characters): ";
    FileInfo file;
    std::cin >> file.Name;
    if (file.Name.size() > 9)
    {
        PrintError("The length of the filename should not exceed 9!");
        return;
    }

    auto fileIter = FindFile(file.Name);
    if (fileIter != m_Files.end())
    {
        PrintError("Filename already exists!");
        return;
    }

    std::cout << "Code length: ";
    std::cin >> file.CodeLength;

    std::cout << "Permission (write read execute): ";
    std::cin >> file.WritePermission 
             >> file.ReadPermission 
             >> file.ExecutePermission;

    m_Files.push_back(file);
    PrintSuccess(std::format(
        "File \"{}\" is created!", 
        file.Name
    ));
}

void FileControlBlock::Delete()
{
    if (m_OpenedFile)
    {
        PrintError("A file is already opened!");
        return;
    }

    std::cout << "Filename: ";
    std::string filename;
    std::cin >> filename;

    auto fileIter = FindFile(filename);
    if (fileIter == m_Files.end())
    {
        PrintError("File does not exist!");
        return;
    }
    m_Files.erase(fileIter);
    PrintSuccess(std::format(
        "File \"{}\" is deleted!", 
        filename
    ));
}

void FileControlBlock::Open()
{
    if (m_OpenedFile)
    {
        PrintError("A file is already opened!");
        return;
    }

    std::cout << "Filename: ";
    std::string filename;
    std::cin >> filename;

    auto fileIter = FindFile(filename);
    if (fileIter == m_Files.end())
    {
        PrintError("File does not exist!");
        return;
    }
    m_OpenedFile = &*fileIter;
    PrintSuccess(std::format(
        "File \"{}\" is opened!", 
        filename
    ));
}

void FileControlBlock::Close()
{
    if (!m_OpenedFile)
    {
        PrintError("No file is opened!");
        return;
    }
    PrintSuccess(std::format(
        "File \"{}\" is closed!",
        m_OpenedFile->Name
    ));
    m_OpenedFile = nullptr;
}

void FileControlBlock::Read() const
{
    if (!m_OpenedFile)
    {
        PrintError("No file is opened!");
        return;
    }
    if (!m_OpenedFile->ReadPermission)
    {
        PrintError(std::format(
            "You don't have the permission to read \"{}\"!",
            m_OpenedFile->Name
        ));
        return;
    }
    PrintSuccess(std::format(
        "File \"{}\" can be read!",
        m_OpenedFile->Name
    ));
}

void FileControlBlock::Write()
{
    if (!m_OpenedFile)
    {
        PrintError("No file is opened!");
        return;
    }
    if (!m_OpenedFile->WritePermission)
    {
        PrintError(std::format(
            "You don't have the permission to write \"{}\"!",
            m_OpenedFile->Name
        ));
        return;
    }
    PrintSuccess(std::format(
        "File \"{}\" can be written!", 
        m_OpenedFile->Name
    ));
}

void FileControlBlock::Execute()
{
    if (!m_OpenedFile)
    {
        PrintError("No file is opened!");
        return;
    }
    if (!m_OpenedFile->ExecutePermission)
    {
        PrintError(std::format(
            "You don't have the permission to execute \"{}\"!",
            m_OpenedFile->Name
        ));
        return;
    }
    PrintSuccess(std::format(
        "File \"{}\" can be executed!",
        m_OpenedFile->Name
    ));
}

bool FileControlBlock::Bye() const
{
    if (m_OpenedFile)
    {
        PrintError("A file is already opened!");
        return false;
    }
    std::cout << "Bye!\n";
    return true;
}

void FileControlBlock::DisplayInfo() const
{
    SetConsoleTextColor(ConsoleTextColor::White);
    if (m_Files.empty())
    {
        std::cout << "No files\n\n";
    }
    else
    {
        std::cout << "Files\n";
        std::cout << std::string(31, '-') << "\n";
        std::cout << "   Name   | W R X | CodeLength\n";
        std::cout << std::string(31, '-') << "\n";
        for (auto& file : m_Files)
        {
            std::cout << std::format("{:<9}", file.Name) << " | "
                      << file.WritePermission << " "
                      << file.ReadPermission << " " 
                      << file.ExecutePermission << " | "
                      << file.CodeLength << "\n";
        }
        std::cout << std::string(31, '-') << "\n\n";
    }
    SetConsoleTextColor(ConsoleTextColor::Gray);
}
