#include "WinSetup.hpp"

#include <iostream>
#include <exception>
#include <filesystem>
#include <fstream>

#include "WinReg/WinReg.hpp"
#include "nlohmann/json.hpp"

#include "Winapi.hpp"
#include "FlagParser.hpp"
#include "Constants.hpp"
#include "JsonFunctions.hpp"

// Setting up registry keys
void Setup(const char **args_begin, const char **args_end)
{
    if(!FlagExists(args_begin, args_end, "-j"))
    {
        InitializeJson(args_begin, args_end);
        std::cout << kJsonName << " generated\n";
    }

    if(!FlagExists(args_begin, args_end, "-r"))
        SetupRegistry();

    if(!FlagExists(args_begin, args_end, "-i"))
        InitialGenerate(args_begin, args_end);
}

void SetupRegistry()
{
    winreg::RegResult res;
    winreg::RegKey key;

    res = key.TryOpen(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP");
    // If key doesn't exist
    if(res.Code() == ERROR_FILE_NOT_FOUND)
    {
        res = key.TryCreate(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP");
        std::cout << "Created registry key \'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP\'\n";
    }
    // If other error was encountered
    else if(!res)
    {
        std::wcout << L"Unable to open registry key \'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP\'\n"
                      L"Error: " << res.ErrorMessage() << '\n';
        TerminateRegistry();
    }
    // If was able to open
    else
        std::cout << "Found registry key \'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP\'\n";

    // If wasn''t able to create or open
    if(!res)
    {
        std::wcout << L"Unable to create registry key \'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP\'\n"
                      L"Error: " << res.ErrorMessage() << '\n';
        TerminateRegistry();
    }

    std::wstring image_path = std::filesystem::current_path().wstring() + L"\\Image.png";
    res = key.TrySetStringValue(L"LockScreenImagePath", image_path);
    // If wasn't able to set value
    if(!res)
    {
        std::wcout << L"Unable to set registry key string value \'LockScreenImagePath\' at key \'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP\'.\n"
                      L"Error: " << res.ErrorMessage() << '\n';
        TerminateRegistry();
    }

    std::wcout << L"Set registry key string value \'LockScreenImagePath\' of \'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP\' to \"" << image_path << L"\"\n";
}

void InitialGenerate(const char **args_begin, const char **args_end)
{
    std::string args(*args_begin);
    args_begin++;
    while(args_begin != args_end)
    {
        args += ' ';
        args += (*args_begin);
        args_begin++;
    }
    
    ExecuteProgram((std::filesystem::current_path().string() + "\\Generate.exe").c_str(), &args[0]);
}

void TerminateRegistry()
{
    std::wcout << L"Termination\n"
                  L"Try opening with administrator rights\n";
    std::cin.ignore();
    std::terminate();
}