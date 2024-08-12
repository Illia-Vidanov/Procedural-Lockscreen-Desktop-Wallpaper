#include "Help.hpp"

#include <iostream>

#include "FlagParser.hpp"
#include "Constants.hpp"


auto HelpCheck(const Flags &flags) -> bool
{
	if(!flags.Contains(kHelpFlag))
		return false;

	std::cout <<
	"General:\n"
	"- \'" << kHelpFlag << "\' - show this list\n"
	"Image:\n"
	"- \'" << kWidthFlag << "=(number)\' and \'" << kHeightFlag << "=(number)\' - set custom width and height for the image respectevly\n"
	"Skips:\n"
	"- \'" << kSkipIntegrityFlag << "\' - skip integrity check\n"
	"- \'" << kSkipJsonFlag << "\' - skip setup of json (use it if you made some changes in json and don't want it to be overwritten)\n"
	"- \'" << kSkipRegistryFlag << "\' - skip setup of registry keys on windows (part that requires administrator rights)\n"
	"- \'" << kSkipGenerateFlag << "\' - skip setup of initial call of Generate.exe (use it if \'Image.png\' already exists and you don't want it to be regenerated)\n"
	"- \'" << kSkipTaskFlag << "\' - skip setup of task in task scheduler (part that requires administrator rights)\n"
	"Clearing (when any of this flags are present, nothing is being set up):\n"
	"- \'" << kClearAllFlag << "\' or \'" << kClearFlag << "\' - remove files, registry keys and task in task scheduler\n"
	"- \'" << kClearFilesFlag << "\' - remove files (props.json and Image.png)\n"
	"- \'" << kClearRegistryFlag << "\' - remove registry keys\n"
	"- \'" << kClearTaskFlag << "\' - remove task from taskscheduler (if custom task name was set specify it using apropriate flag \'-task-name=(task name)\'. You can also specify custom powershell path)\n"
	"Task scheduler:\n"
	"- \'" << kUserNameFlag << "=(user name)\' - set user name who's login will change lock screen image (if empty login of any user will generate new image)\n"
	"- \'" << kPowershellPathFlag << "=(path to powershell.exe)\' - set custom path to powershell executable, default path is \'C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe\' (currently it is required to setup task in task scheduler)\n"
	"- \'" << kTaskNameFlag << "=(task name)\' - set custom name for the task in task scheduler (I personally don't know who will use this feature, but for people who use task scheduler it might be useful. By default task name is \'Generate Wallpaper\')\n";

	return true;
}