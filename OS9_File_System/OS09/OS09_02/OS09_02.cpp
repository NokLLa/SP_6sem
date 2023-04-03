#include <iostream>
#include <cstdlib>
#include "Windows.h"


BOOL delRowFileTxt(LPWSTR FileName, DWORD row);
BOOL printFileTxt(LPWSTR FileName);
BOOL delRowFileTxt(LPWSTR FileName, DWORD row);
int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    LPWSTR fileName = (LPWSTR)L"C:\\Users\\nk\\Desktop\\SP\\OS9_File_System\\OS09_01.txt";


    printFileTxt(fileName);

    delRowFileTxt(fileName, 1);
    delRowFileTxt(fileName, 3);
    delRowFileTxt(fileName, 8);
    delRowFileTxt(fileName, 10);

    printFileTxt(fileName);

}

BOOL delRowFileTxt(LPWSTR FileName, DWORD row)
{
    PLARGE_INTEGER fileSize = new LARGE_INTEGER();
    int rowCount = 1;
    int position = 0;
    int positionAfter = 0;
    bool rowFound = false;

    try
    {
        if (row <= 0)
        {
            throw "Invalid number of raw";
        }
        HANDLE of = CreateFile(
            FileName,
            GENERIC_READ | GENERIC_WRITE,
            NULL,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (of == INVALID_HANDLE_VALUE)
        {
            CloseHandle(of);
            throw "Open file failed";
        }



        if (GetFileSizeEx(of, fileSize))
        {
            char* buf = new char[fileSize->QuadPart + 1];
            char* bufAfterDel = new char[fileSize->QuadPart + 1];

            ZeroMemory(buf, fileSize->QuadPart);
            ZeroMemory(bufAfterDel, fileSize->QuadPart);

            DWORD n = NULL;

            if (ReadFile(of, buf, fileSize->QuadPart, &n, NULL))
            {
                buf[fileSize->QuadPart] = '\0';
                while (buf[position] != '\0')
                {

                    if (buf[position] == '\n')
                    {
                        rowCount++;
                    }


                    if (rowCount == row)
                    {
                        rowFound = true;
                        rowCount++;
                        position++;
                        while (buf[position] != '\n' && buf[position] != '\0')
                        {
                            position++;
                        }
                        if (row == 1)
                        {
                            position++;
                        }
                    }

                    bufAfterDel[positionAfter++] = buf[position++];

                    if (buf[position] == '\0')
                    {
                        break;
                    }

                }
                bufAfterDel[positionAfter] = '\0';
            }

            if (rowFound)
            {
                std::cout << "\n--- After deletion row " << row << std::endl << bufAfterDel << std::endl;

                if (SetFilePointer(of, 0, 0, FILE_BEGIN) == 0)
                {
                    if (WriteFile(of, bufAfterDel, positionAfter, &n, NULL))
                    {
                        std::cout << "Row deleted successfully" << std::endl;
                    }
                    else
                    {
                        CloseHandle(of);
                        throw "WriteFile failed";
                    }
                    if (!SetEndOfFile(of))
                    {
                        CloseHandle(of);
                        throw "SetEndOfFile failed";
                    }
                }
                else
                {
                    CloseHandle(of);
                    throw "SetFilePointer failed";
                }
            }
            else
            {
                CloseHandle(of);
                throw "Row isn't found";
            }
        }


        else
        {
            CloseHandle(of);
            throw "GetFileSizeEx failed";
        }
        CloseHandle(of);
    }
    catch (const char* err)
    {
        std::cout << "--- Error:\n" << err << "\n";
        return false;
    }
    return true;
}









BOOL printFileTxt(LPWSTR FileName)
{
    LPBY_HANDLE_FILE_INFORMATION lpFileInformation = new BY_HANDLE_FILE_INFORMATION();
    DWORD fileType;
    PLARGE_INTEGER fileSize = new LARGE_INTEGER();
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
            CloseHandle(of);
            throw "Open file failed";
        }

        if (GetFileSizeEx(of, fileSize))
        {
            char* buf = new char[fileSize->QuadPart];
            ZeroMemory(buf, sizeof(buf));
            DWORD n = NULL;
            if (ReadFile(of, buf, fileSize->QuadPart, &n, NULL))
            {
                buf[fileSize->QuadPart] = '\0';
                std::cout << "\n-- File text: " << n << " byte successful: \n" << buf << "\n";
            }
            else
            {
                CloseHandle(of);
                throw "ReadFile failed";
            }
        }
        else
        {
            CloseHandle(of);
            throw "GetFileSizeEx failed";
        }
        CloseHandle(of);
    }
    catch (const char* err)
    {
        std::cout << "--- Error:\n" << err << "\n";
        return false;
    }
    return true;
}
