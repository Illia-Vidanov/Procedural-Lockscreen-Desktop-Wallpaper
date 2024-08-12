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
	"- \'" << kWidthFlag << "=(number)\' and \'" << kHeightFlag << "=(number)\' - set custom width and height for the image respectevly\n";

	return true;
}