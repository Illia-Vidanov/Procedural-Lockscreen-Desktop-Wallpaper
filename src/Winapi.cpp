#include "WinApi.hpp"

#include <windows.h>
#include <iostream>
#include <exception>
#include <algorithm>

#include "StringUtils.hpp"


auto ExecuteProgram(const char *program_path, char *args) -> int
{
    wchar_t *program_path_w = Utf8ToWideChar(program_path);
    wchar_t *args_w = Utf8ToWideChar(args);
    std::replace(program_path_w, program_path_w + wcslen(program_path_w), L'/', L'\\');
    std::replace(args_w, args_w + wcslen(args_w), L'/', L'\\');

    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    HANDLE h_read, h_write;
    if(!CreatePipe(&h_read, &h_write, &sa, 0))
    {
        std::cout << "Wasn't able to create anonymous pipe\n";
        TerminateAdmin();
    }

    PROCESS_INFORMATION pi;
    STARTUPINFOW si;

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));

    si.cb = sizeof(STARTUPINFOA);
    si.hStdOutput = h_write;
    si.dwFlags |= STARTF_USESTDHANDLES;

    if(!CreateProcessAsUserW(
        NULL,
        program_path_w,
        args_w,
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
        std::wcout << L"Wasn't able to create child process in order to open \"" << program_path << L"\" with arguments \"" << args << "\" with error " << GetLastError() << '\n'
                   << L"Termination";
        std::terminate();
    }

    //WaitForSingleObject(&pi, INFINITE);
    CloseHandle(h_write);

    char buffer[4096];
    DWORD bytesRead;
    while(ReadFile(h_read, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
        std::cout.write(buffer, bytesRead);
    
    long unsigned int exit_code = 0; // what is 'long unsigned int'???? It's not uint32_t and not uint64_t
    GetExitCodeProcess(pi.hProcess, &exit_code);

    CloseHandle(h_read);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    delete[] program_path_w;
    delete[] args_w;
    
    return exit_code;
}

void GetScreenResolution(int &width, int &height)
{
    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);
}

void TerminateAdmin()
{
    std::wcout << L"Termination\n"
                  L"Try opening with administrator rights\n";
    std::terminate();
}

auto Utf8ToWideChar(const char *str) -> wchar_t*
{
    const int string_size = strlen(str) + 1; // + 1 to account for \0
    const int buffer_size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, string_size, nullptr, 0);
    wchar_t *result = new wchar_t[buffer_size];

    if(!MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, string_size, result, buffer_size))
    {
        std::cout << "Wasn't able to convert \'" << str << "\' to wide char\n"
                     "Terminating";
        std::terminate();
    }

    return result;
}

auto WideCharToUtf8(const wchar_t *str) -> char*
{
    const int string_size = StrLen(str) + 1; // + 1 to account for \0
    const int buffer_size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str, string_size, nullptr, 0, NULL, NULL);
    char *result = new char[buffer_size];

    if(!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str, string_size, result, buffer_size, NULL, NULL))
    {
        std::wcout << L"Wasn't able to convert \'" << str << L"\' to utf8\n"
                      L"Terminating";
        std::terminate();
    }

    return result;
}

auto ConvertArgsToUtf8(int argc, const char **argv) -> const char**
{
    const char **utf8_args = new const char *[argc];
    for(int i = 0; i < argc; i++)
    {
        const int string_size = strlen(argv[i]) + 1; // + 1 to account for \0
        const int buffer_size = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, argv[i], string_size, nullptr, 0);
        wchar_t *result = new wchar_t[buffer_size];

        if(!MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, argv[i], string_size, result, buffer_size))
        {
            std::cout << "Wasn't able to convert argument \'" << argv[i] << "\' to wide char\n"
                        "Terminating";
            std::terminate();
        }

        utf8_args[i] = WideCharToUtf8(result);
        delete[] result;
    }

    return utf8_args;
}
