#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

class Filesystem {
public:
    void printCWD();

    std::wstring openFolderDialog();
    std::wstring openFileDialog();
    void saveFileDialog();
    void makeDirectory(const std::wstring& path, const std::wstring& dirName = L"Untitled");
    std::string getParentDirectory(const std::string& filepath);
    std::string getFilename(const std::string& filepath);
};

extern Filesystem gFilesystem;


#endif 