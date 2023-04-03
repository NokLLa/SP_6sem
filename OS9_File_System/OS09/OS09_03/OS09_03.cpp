#include <iostream>
#include <cstdlib>
#include "Windows.h"


BOOL insRowFileTxt(LPWSTR FileName, LPWSTR str, DWORD row);
BOOL printFileTxt(LPWSTR FileName);

int main()
{
    setlocale(LC_ALL, "ru-RU.utf8");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    LPWSTR fileName = (LPWSTR)L"C:\\Users\\nk\\Desktop\\SP\\OS9_File_System\\OS09_01.txt";

    insRowFileTxt(fileName, (LPWSTR)L"0.Тест", 0);
    insRowFileTxt(fileName, (LPWSTR)L"-1. Minus", -1);
    insRowFileTxt(fileName, (LPWSTR)L"4. Тестовая строка", 5);
    insRowFileTxt(fileName, (LPWSTR)L"7. Семь", 7);

    printFileTxt(fileName);
}

BOOL insRowFileTxt(LPWSTR FileName, LPWSTR str, DWORD row)
{
    PLARGE_INTEGER fileSize = new LARGE_INTEGER();
    int rowCount = 1;
    int position = 0;
    int positionAfter = 0;
    bool rowFound = false;

    try
    {
        HANDLE of = CreateFile(
            FileName,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (of == INVALID_HANDLE_VALUE)
            throw "Open file failed";




        if (GetFileSizeEx(of, fileSize))
        {
            int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
            char* cstr = new char[len];
            WideCharToMultiByte(CP_UTF8, 0, str, -1, cstr, len, NULL, NULL);

            char* buf = new char[(fileSize->QuadPart + 1)];
            char* bufAfterIns = new char[(fileSize->QuadPart + 1 + strlen(cstr))];

            ZeroMemory(buf, (fileSize->QuadPart + 1));
            ZeroMemory(bufAfterIns, (fileSize->QuadPart + 1 + strlen(cstr)));

            DWORD n = NULL;

            if (ReadFile(of, buf, fileSize->QuadPart, &n, NULL))
            {
                buf[fileSize->QuadPart] = '\0';
                while (buf[position] != '\0')
                {
                    if ((rowCount == row || row == 0) && !rowFound)
                    {
                        for (int i = 0; i < strlen(cstr); i++)
                        {
                            bufAfterIns[positionAfter++] = cstr[i];
                        }
                        bufAfterIns[positionAfter++] = '\r';
                        bufAfterIns[positionAfter++] = '\n';
                        rowFound = true;
                        rowCount++;
                    }
                    if (buf[position] == '\n')
                    {
                        rowCount++;
                    }
                    bufAfterIns[positionAfter++] = buf[position++];
                    if (buf[position] == '\0')
                    {
                        break;
                    }
                }
                if (row == MAXDWORD)
                {
                    bufAfterIns[positionAfter++] = '\r';
                    bufAfterIns[positionAfter++] = '\n';
                    for (int i = 0; i < strlen(cstr); i++)
                    {
                        bufAfterIns[positionAfter++] = cstr[i];
                    }
                    rowFound = true;
                    rowCount++;
                }
                bufAfterIns[positionAfter] = '\0';
            }


            if (rowFound)
            {
                printf("\n--- After insertion row %d:\n%s\n", row == MAXDWORD ? -1 : row, bufAfterIns);
                if (SetFilePointer(of, 0, 0, FILE_BEGIN) == 0)
                {
                    if (WriteFile(of, bufAfterIns, strlen(bufAfterIns), &n, NULL))
                    {
                        printf("\nRow inserted successfully\n");
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
            char* buf = new char[fileSize->QuadPart * sizeof(char)];
            ZeroMemory(buf, sizeof(buf));
            DWORD n = NULL;
            if (ReadFile(of, buf, fileSize->QuadPart, &n, NULL))
            {
                buf[fileSize->QuadPart] = '\0';
                printf("\n-- File text:%d byte successful:\n%s\n", n, buf);
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