#include "Winapi.hpp"

#include <windows.h>
#include <iostream>

void ExecuteProgram(const char *program_path, char *args)
{
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    HANDLE h_read, h_write;
    if(!CreatePipe(&h_read, &h_write, &sa, 0))
    {
        std::cout << "Wasn't able to create anonymous pipe\n";
        TerminateExecute();
    }

    PROCESS_INFORMATION pi;
    STARTUPINFOA si;

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));

    si.cb = sizeof(STARTUPINFOA);
    si.hStdOutput = h_write;
    si.dwFlags |= STARTF_USESTDHANDLES;

    if(!CreateProcessA(
        program_path,
        args,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    ))
    {
        std::cout << "Wasn't able to create child process in order to open: " << program_path << '\n';
        TerminateExecute();
    }

    //WaitForSingleObject(&pi, INFINITE);
    CloseHandle(h_write);

    char buffer[4096];
    DWORD bytesRead;
    while(ReadFile(h_read, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
        std::cout.write(buffer, bytesRead);
    
    CloseHandle(h_read);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void GetScreenResolution(int &width, int &height)
{
    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);
}

void MySleep(int time_seconds)
{
    Sleep(time_seconds * 1000);
}

void TerminateExecute()
{
    std::wcout << L"Termination\n"
                  L"Try opening with administrator rights\n";
    std::cin.ignore();
    std::terminate();
}