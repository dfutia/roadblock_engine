#include "filesystem.h"

#include <iostream>
#include <filesystem>
#include <windows.h>
#include <commdlg.h>
#include <shobjidl_core.h>
#include <sstream>

Filesystem gFilesystem;

void Filesystem::printCWD() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current working directory: " << currentPath << std::endl;
}

void Filesystem::makeDirectory(const std::wstring& path, const std::wstring& dirName) {
    std::wstring directory = path;
    if (directory.back() != L'\\' && directory.back() != L'/') {
        directory += L'\\';
    }

    std::wstring finalDirName = dirName;
    std::wstring fullPath = directory + finalDirName;
    int counter = 1;

    // Keep incrementing the directory name if it already exists
    while (std::filesystem::exists(fullPath)) {
        std::wstringstream ss;
        ss << dirName << L" (" << counter << L")";
        finalDirName = ss.str();
        fullPath = directory + finalDirName;
        counter++;
    }

    if (CreateDirectoryW(fullPath.c_str(), NULL)) {
        std::wcout << L"Project directory created: " << fullPath << std::endl;
    }
    else {
        DWORD error = GetLastError();
        std::wcerr << L"Failed to create directory: " << fullPath << L". Error code: " << error << std::endl;
    }
}

std::wstring Filesystem::openFolderDialog() {
    std::wstring folderPath;

    // Initialize COM library
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileDialog* pFileDialog = nullptr;

        // Create the FileOpenDialog object
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

        if (SUCCEEDED(hr)) {
            // Set options to select folders only
            DWORD dwOptions;
            pFileDialog->GetOptions(&dwOptions);
            pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

            // Show the dialog
            hr = pFileDialog->Show(NULL);

            // Get the folder selected by the user
            if (SUCCEEDED(hr)) {
                IShellItem* pItem = nullptr;
                hr = pFileDialog->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Store the selected folder path
                    if (SUCCEEDED(hr)) {
                        folderPath = pszFilePath;
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileDialog->Release();
        }
        CoUninitialize();
    }

    return folderPath;
}

std::wstring Filesystem::openFileDialog() {
    try {
        wchar_t filename[MAX_PATH];

        OPENFILENAMEW ofn;
        ZeroMemory(&filename, sizeof(filename));
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
        ofn.lpstrFilter = L"Text Files\0*.txt\0Any File\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = L"Select a File, yo!";
        ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameW(&ofn))
        {
            std::wcout << L"You chose the file \"" << filename << L"\"\n";
            return std::wstring(filename);
        }
        else {
            switch (CommDlgExtendedError())
            {
            case CDERR_DIALOGFAILURE: std::wcout << L"CDERR_DIALOGFAILURE\n";   break;
            case CDERR_FINDRESFAILURE: std::wcout << L"CDERR_FINDRESFAILURE\n";  break;
            case CDERR_INITIALIZATION: std::wcout << L"CDERR_INITIALIZATION\n";  break;
            case CDERR_LOADRESFAILURE: std::wcout << L"CDERR_LOADRESFAILURE\n";  break;
            case CDERR_LOADSTRFAILURE: std::wcout << L"CDERR_LOADSTRFAILURE\n";  break;
            case CDERR_LOCKRESFAILURE: std::wcout << L"CDERR_LOCKRESFAILURE\n";  break;
            case CDERR_MEMALLOCFAILURE: std::wcout << L"CDERR_MEMALLOCFAILURE\n"; break;
            case CDERR_MEMLOCKFAILURE: std::wcout << L"CDERR_MEMLOCKFAILURE\n";  break;
            case CDERR_NOHINSTANCE: std::wcout << L"CDERR_NOHINSTANCE\n";     break;
            case CDERR_NOHOOK: std::wcout << L"CDERR_NOHOOK\n";          break;
            case CDERR_NOTEMPLATE: std::wcout << L"CDERR_NOTEMPLATE\n";      break;
            case CDERR_STRUCTSIZE: std::wcout << L"CDERR_STRUCTSIZE\n";      break;
            case FNERR_BUFFERTOOSMALL: std::wcout << L"FNERR_BUFFERTOOSMALL\n";  break;
            case FNERR_INVALIDFILENAME: std::wcout << L"FNERR_INVALIDFILENAME\n"; break;
            case FNERR_SUBCLASSFAILURE: std::wcout << L"FNERR_SUBCLASSFAILURE\n"; break;
            default: std::wcout << L"You cancelled.\n";
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::wcerr << L"Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return L"";
}

void Filesystem::saveFileDialog() {
    try {
        wchar_t filename[MAX_PATH];

        OPENFILENAMEW ofn;
        ZeroMemory(&filename, sizeof(filename));
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
        ofn.lpstrFilter = L"Text Files\0*.txt\0Any File\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = L"Save the File";
        ofn.Flags = OFN_DONTADDTORECENT | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;  // Important flag for save dialog

        if (GetSaveFileNameW(&ofn))
        {
            std::wcout << L"You chose to save the file \"" << filename << L"\"\n";
        }
        else {
            switch (CommDlgExtendedError())
            {
            case CDERR_DIALOGFAILURE: std::wcout << L"CDERR_DIALOGFAILURE\n";   break;
            case CDERR_FINDRESFAILURE: std::wcout << L"CDERR_FINDRESFAILURE\n";  break;
            case CDERR_INITIALIZATION: std::wcout << L"CDERR_INITIALIZATION\n";  break;
            case CDERR_LOADRESFAILURE: std::wcout << L"CDERR_LOADRESFAILURE\n";  break;
            case CDERR_LOADSTRFAILURE: std::wcout << L"CDERR_LOADSTRFAILURE\n";  break;
            case CDERR_LOCKRESFAILURE: std::wcout << L"CDERR_LOCKRESFAILURE\n";  break;
            case CDERR_MEMALLOCFAILURE: std::wcout << L"CDERR_MEMALLOCFAILURE\n"; break;
            case CDERR_MEMLOCKFAILURE: std::wcout << L"CDERR_MEMLOCKFAILURE\n";  break;
            case CDERR_NOHINSTANCE: std::wcout << L"CDERR_NOHINSTANCE\n";     break;
            case CDERR_NOHOOK: std::wcout << L"CDERR_NOHOOK\n";          break;
            case CDERR_NOTEMPLATE: std::wcout << L"CDERR_NOTEMPLATE\n";      break;
            case CDERR_STRUCTSIZE: std::wcout << L"CDERR_STRUCTSIZE\n";      break;
            case FNERR_BUFFERTOOSMALL: std::wcout << L"FNERR_BUFFERTOOSMALL\n";  break;
            case FNERR_INVALIDFILENAME: std::wcout << L"FNERR_INVALIDFILENAME\n"; break;
            case FNERR_SUBCLASSFAILURE: std::wcout << L"FNERR_SUBCLASSFAILURE\n"; break;
            default: std::wcout << L"You cancelled.\n";
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::wcerr << L"Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::string Filesystem::getParentDirectory(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.parent_path().string();
}

std::string Filesystem::getFilename(const std::string& filepath) {
    size_t lastSlash = filepath.find_last_of("/\\");
    size_t lastDot = filepath.find_last_of(".");

    if (lastSlash == std::string::npos) {
        lastSlash = 0;
    }
    else {
        lastSlash++;
    }

    if (lastDot == std::string::npos || lastDot < lastSlash) {
        return filepath.substr(lastSlash);
    }

    return filepath.substr(lastSlash, lastDot - lastSlash);
}
