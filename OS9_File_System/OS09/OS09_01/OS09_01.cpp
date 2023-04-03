#include <iostream>
#include <cstdlib>
#include "Windows.h"

using namespace std;

BOOL printFileInfo(LPWSTR FileName);
BOOL printFileTxt(LPWSTR FileName);

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    printFileInfo((LPWSTR)L"C:\\Users\\nk\\Desktop\\SP\\OS9_File_System\\OS09_01.txt");
    printFileTxt((LPWSTR)L"C:\\Users\\nk\\Desktop\\SP\\OS9_File_System\\OS09_01.txt");
    system("pause");
}

BOOL printFileInfo(LPWSTR FileName)
{
    LPBY_HANDLE_FILE_INFORMATION lpFileInformation = new BY_HANDLE_FILE_INFORMATION();
    LPSYSTEMTIME systemTimeCreation = new SYSTEMTIME();
    LPSYSTEMTIME systemTimeLastWrite = new SYSTEMTIME();
    PLARGE_INTEGER fileSize = new LARGE_INTEGER();
    DWORD fileType;

    try
    {
        HANDLE of = CreateFile(
            FileName,
            GENERIC_WRITE | GENERIC_READ,
            NULL,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (of == INVALID_HANDLE_VALUE)
        {
            throw "Open file failed";
        }

        wcout << L"--- Open file successful --- \n\n";
        wcout << L"--- File info:  --- \n";

        
        char* name = new char[MAX_PATH];
        int i = wcslen(FileName) - 1;
        int lenName = 0;
        while (FileName[i--] != '\\');
        {
            i++;
            while (FileName[i++] != '\0')
            {
                name[lenName++] = FileName[i];
            }
        }
        printf("File name:%s \n", name);

        if (GetFileInformationByHandle(of, lpFileInformation))
        {
            if (FileTimeToSystemTime(&lpFileInformation->ftCreationTime, systemTimeCreation) // filetime -> systemtime
                && FileTimeToSystemTime(&lpFileInformation->ftLastAccessTime, systemTimeLastWrite))
            {
                printf("Creation time: %u.%u.%u %u:%u:%u \n", systemTimeCreation->wDay, systemTimeCreation->wMonth, systemTimeCreation->wYear, systemTimeCreation->wHour, systemTimeCreation->wMinute, systemTimeCreation->wSecond);
                printf("Last access time: %u.%u.%u %u:%u:%u \n", systemTimeLastWrite->wDay, systemTimeLastWrite->wMonth, systemTimeLastWrite->wYear, systemTimeLastWrite->wHour, systemTimeLastWrite->wMinute, systemTimeLastWrite->wSecond);
            }
            else
            {
                throw "FileTimeToSystemTime failed";
            }
        }
        else
        {
            throw "GetFileInformationByHandle failed";
        }

        cout << "File type: ";
        if (fileType = GetFileType(of))
        {
            switch (fileType)
            {
            case FILE_TYPE_CHAR: cout << "Printer, console file\n"; break;
            case FILE_TYPE_DISK: cout << "Disk file\n"; break;
            case FILE_TYPE_PIPE: cout << "Socket,named pipe, or anonymous pipe\n"; break;
            case FILE_TYPE_REMOTE: cout << "reserved\n"; break;
            case FILE_TYPE_UNKNOWN: cout << "Unknown file type\n"; break;
            }
        }
        else
        {
            throw "GetFileType failed";
        }

        if (GetFileSizeEx(of, fileSize))
        {
            cout << "File size: " << fileSize->QuadPart << " bytes" << endl;
        }
        else
        {
            throw "GetFileSizeEx failed";
        }

        CloseHandle(of);
    }
    catch (char* err)
    {
        cout << "--- Error:\n" << err << "\n";
        return false;
    }
    return true;
}

BOOL printFileTxt(LPWSTR FileName)
{
    LPBY_HANDLE_FILE_INFORMATION lpFileInformation = new BY_HANDLE_FILE_INFORMATION();
    PLARGE_INTEGER fileSize = new LARGE_INTEGER();
    DWORD fileType;
    try
    {
        HANDLE of = CreateFile(
            FileName,
            GENERIC_READ,
            NULL,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (of == INVALID_HANDLE_VALUE)
        {
            throw "Open file failed";
        }

        if (GetFileSizeEx(of, fileSize))
        {
            char* buf = new char[fileSize->QuadPart];
            ZeroMemory(buf, fileSize->QuadPart);

            DWORD n = NULL;

            if (ReadFile(
                of, 
                buf, 
                fileSize->QuadPart,
                &n,
                NULL))
            {
                buf[fileSize->QuadPart] = '\0';
                cout << "\n-- File text: \t" << n << " byte successful: \n" << buf << "\n";
            }
        }
        else
        {
            throw "GetFileSizeEx failed";
        }
        CloseHandle(of);
    }
    catch (char* err)
    {
        cout << "--- Error:\n" << err << "\n";
        return false;
    }
    return true;
}