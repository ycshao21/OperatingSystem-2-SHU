#pragma once
#include <string>
#include <iostream>
#include <vector>

struct FileInfo
{
    std::string Name;
    int WritePermission = 1;
    int ReadPermission = 1;
    int ExecutePermission = 1;
    uint32_t CodeLength = 555;
};

class FileControlBlock
{
public:
    FileControlBlock() = default;
    ~FileControlBlock() = default;

    void DisplayInfo() const;

    void Create();
    void Delete();
    void Open();
    void Close();
    void Read() const;
    void Write();
    void Execute();
    bool Bye() const;
private:
    auto FindFile(const std::string& filename) const
    {
        return std::find_if(m_Files.begin(), m_Files.end(),
            [&filename](const FileInfo& file) { return file.Name == filename; }
        );
    }
    auto FindFile(const std::string& filename)
    {
        return std::find_if(m_Files.begin(), m_Files.end(),
            [&filename](FileInfo& file) { return file.Name == filename; }
        );
    }
private:
    std::vector<FileInfo> m_Files;
    FileInfo* m_OpenedFile = nullptr;
};

