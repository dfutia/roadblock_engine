#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

class Filesystem {
public:
    void printCWD();

    std::wstring openFolderDialog();

    void openFileDialog();
    void saveFileDialog();

    void makeDirectory(const std::wstring& path, const std::wstring& dirName = L"Untitled");
};

extern Filesystem gFilesystem;


#endif 