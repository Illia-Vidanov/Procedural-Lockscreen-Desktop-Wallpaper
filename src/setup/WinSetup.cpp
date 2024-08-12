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
#include "JsonParser.hpp"
#include "StringUtils.hpp"


// Setting up registry keys
void Setup(const Flags &flags)
{
    if(!flags.Contains(kSkipIntegrityFlag))
        CheckIntegrity();

    if(!flags.Contains(kSkipJsonFlag))
        InitializeJson(flags);

    if(!flags.Contains(kSkipRegistryFlag))
        SetupRegistry();

    if(!flags.Contains(kSkipGenerateFlag))
        InitialGenerate(flags); // Handels output on its own

    if(!flags.Contains(kSkipTaskFlag))
        AddTaskToScheduler(flags);

    std::cout << "Everything is setup\n";
}

void CheckIntegrity()
{
    if(!std::filesystem::exists(kGenerateName))
    {
        std::cout << kGenerateName << " is not in the same folder as Setup.exe\n"
                    "Terminating";
        std::terminate();
    }

    std::cout << "Integrity checked\n";
}

void SetupRegistry()
{
    winreg::RegResult res;
    winreg::RegKey key;

    res = key.TryOpen(HKEY_LOCAL_MACHINE, kPersonalizationFolder);
    // If key doesn't exist
    if(res.Code() == ERROR_FILE_NOT_FOUND)
        res = key.TryCreate(HKEY_LOCAL_MACHINE, kPersonalizationFolder);
    // If other error was encountered
    else if(!res)
    {
        std::wcout << L"Unable to open registry folder \'" << kPersonalizationFolder << L"\n"
                      L"Error: " << res.ErrorMessage() << L"\n"
                      L"Try opening with administrator rights\n";
        return;
    }

    // If wasn't able to create or open
    if(!res)
    {
        std::wcout << L"Unable to create registry folder \'" << kPersonalizationFolder << L"\n"
                      L"Error: " << res.ErrorMessage() << '\n';
        return;
    }

    const std::wstring image_path = AddCurrentPathToString(kImageNameW);
    std::replace(image_path.begin(), image_path.end(), '/', '\\');
    
    res = key.TrySetStringValue(kImagePathKey, image_path);
    // If wasn't able to set value
    if(!res)
    {
        std::wcout << L"Unable to set registry key string value " << kImagePathKey << " at folder \'" << kPersonalizationFolder << L"\n"
                      L"Error: " << res.ErrorMessage() << '\n';
        return;
    }

    std::cout << "Registry keys set up\n";
}

void InitialGenerate(const Flags &flags)
{
    if(ExecuteProgram((AddCurrentPathToString(kGenerateName)).c_str(), &flags.GetArgsAsString()[0]))
        std::cout << "Error generating initial image\n";
}

void AddTaskToScheduler(const Flags &flags)
{
    // At first I wanted to use windows' library (taskschd.h), but it turned out that mingw-w64 tries to be similar to linux systems so this library isn't full here (it misses ILogonTrigger)
    // So I found a workaround by calling a powershell function instead

    const std::string task_name = flags.Contains(kTaskNameFlag) ? flags.Get(kTaskNameFlag) : kDefTaskName;
    const std::string powershell_path = flags.Contains(kPowershellPathFlag) ? flags.Get(kPowershellPathFlag) : kDefPowershellPath;

    std::string args("-Command \"Unregister-ScheduledTask -TaskName \\\"");
    args += task_name;
    args += "\\\" -Confirm:$false -ErrorAction SilentlyContinue; Register-ScheduledTask -TaskName \\\"";
    args += task_name;
    args += "\\\" -Trigger (New-CimInstance -CimClass (Get-CimClass -Namespace ROOT\\Microsoft\\Windows\\TaskScheduler -ClassName MSFT_TaskSessionStateChangeTrigger) -Property @{StateChange = 8";

    if(flags.Contains(kUserNameFlag))
    {
        args += "; UserId = \\\"";
        args += flags.Get(kUserNameFlag);
        args += "\\\"";
    }

    args += "} -ClientOnly) -Action (New-ScheduledTaskAction -Execute \\\"";
    args += kGenerateName;
    args += "\\\" -WorkingDirectory \\\"";
    args += std::filesystem::current_path().string();
    args += "\\\") -Settings (New-ScheduledTaskSettingsSet -AllowStartIfOnBatteries -StartWhenAvailable)\"";
    ExecuteProgram(powershell_path.c_str(), &args[0]);

    std::cout << "Task added to task scheduler\n";
}