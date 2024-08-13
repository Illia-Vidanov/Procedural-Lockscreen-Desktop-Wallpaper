#include "WinClear.hpp"

#include <iostream>
#include <filesystem>

#include "WinReg/WinReg.hpp"

#include "FlagParser.hpp"
#include "Constants.hpp"
#include "Api.hpp"


auto ClearCheck(const Flags & flags) -> bool
{
	bool clearing = false;

	if(flags.Contains(kClearFlag) || flags.Contains(kClearAllFlag))
	{
		RemoveFiles();
		RemoveRegistry();
		RemoveTask(flags);

		std::cout << "Everything is cleared!\n";
		return true;
	}

	if(flags.Contains(kClearFilesFlag))
    {
        clearing = true;
        RemoveFiles();
    }

    if(flags.Contains(kClearRegistryFlag))
    {
        clearing = true;
        RemoveRegistry();
    }

    if(flags.Contains(kClearTaskFlag))
    {
        clearing = true;
        RemoveTask(flags);
    }

	return clearing;
}

void RemoveFiles()
{
    std::filesystem::remove(kJsonName);
    std::filesystem::remove(kImageName);
    std::cout << "Files removed\n";
}

void RemoveRegistry()
{
    winreg::RegResult res;
    winreg::RegKey key;

    res = key.TryOpen(HKEY_LOCAL_MACHINE, kPersonalizationFolder);
    if(res.Code() == ERROR_FILE_NOT_FOUND)
        return;
    else if(!res)
    {
        std::wcout << L"Unable to open registry folder to delete registry key \'" << kPersonalizationFolder << L"\'\n"
                      L"Error: " << res.ErrorMessage() << L"\n"
                      L"Try opening with administrator rights\n";
        return;
    }

    res = key.TryDeleteValue(kImagePathKey);
    if(!res)
    {
        std::wcout << L"Unable to delete registry value \'" << kImagePathKey << L"\'\n"
                      L"Error: " << res.ErrorMessage() << L"\n";
        return;
    }

    std::cout << "Registry keys removed\n";
}

void RemoveTask(const Flags &flags)
{
    const std::string task_name = flags.Contains(kTaskNameFlag) ? flags.Get(kTaskNameFlag) : kDefTaskName;
    const std::string powershell_path = flags.Contains(kPowershellPathFlag) ? flags.Get(kPowershellPathFlag) : kDefPowershellPath;

    std::string args("-Command \"Unregister-ScheduledTask -TaskName \\\"");
    args += task_name;
    args += "\\\" -Confirm:$false -ErrorAction SilentlyContinue\"";
    ExecuteProgram(powershell_path.c_str(), &args[0]);

    std::cout << "Task removed\n";
}
