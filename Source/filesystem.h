#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

class Filesystem {
public:
    std::string openFileDialog();
};

extern Filesystem gFilesystem;


#endif 