#include <iostream>
#include <cstdlib>
#include "Windows.h"

BOOL printWatchRowFileTxt(LPWSTR FileName, DWORD mlsec);

int main()
{
    setlocale(LC_ALL, "ru-RU.utf8");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    LPWSTR fileName = (LPWSTR)L"C:\\Users\\nk\\Desktop\\SP\\OS9_File_System\\OS09_01.txt";
    printWatchRowFileTxt(fileName, 50000);
}

BOOL printWatchRowFileTxt(LPWSTR FileName, DWORD mlsec)
{
    PLARGE_INTEGER fileSize = new LARGE_INTEGER();

    LPWSTR path = (LPWSTR)L"C:\\Users\\nk\\Desktop\\SP\\OS9_File_System\\";

    printf("File catalog:%s \n", path);

    try
    {
        HANDLE notif = FindFirstChangeNotification(path, false, FILE_NOTIFY_CHANGE_LAST_WRITE);
        DWORD err = GetLastError();
        clock_t t1 = clock();
        clock_t t2 = clock();
        DWORD dwWaitStatus;
        printf("\nWatch started timestamp %d:", t1);
        while (true)
        {
            dwWaitStatus = WaitForSingleObject(notif, mlsec);
            switch (dwWaitStatus)
            {
                case WAIT_OBJECT_0:
                {
                    if (FindNextChangeNotification(notif) == FALSE)
                    {
                        break;
                    }
                    else
                    {
                        int position = 0;
                        int rowCount = 0;
                        HANDLE of = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,  NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                        if (of == INVALID_HANDLE_VALUE)
                        {
                            CloseHandle(of);
                            throw "Open file failed";
                        }


                        if (GetFileSizeEx(of, fileSize))
                        {
                            char* buf = new char[(fileSize->QuadPart + 1) * sizeof(char)];
                            ZeroMemory(buf, (fileSize->QuadPart + 1) * sizeof(char));

                            DWORD n = NULL;

                            if (ReadFile(of, buf, fileSize->QuadPart, &n, NULL))
                            {
                                while (buf[position++] != '\0')
                                {
                                    if (buf[position] == '\n')
                                    {
                                        rowCount++;
                                    }
                                }
                                if (position > 0 && buf[position - 1] != '\n')
                                {
                                    rowCount++;
                                }
                            }
                        }
                        printf("\n Row quantity: %d", rowCount);
                        CloseHandle(of);
                    }
                }
            }
            t2 = clock();
            if (t2 - t1 > mlsec)
            {
                break;
            }
            mlsec -= (t2 - t1);
        }
        CloseHandle(notif);
        printf("\n Watch ended timestamp %d:", t2);
    }
    catch (const char* err)
    {
        std::cout << "--- Error:\n" << err << "\n";
        return false;
    }
    return true;
}
