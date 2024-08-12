#include "Help.hpp"

#include <iostream>

#include "FlagParser.hpp"
#include "Constants.hpp"


auto HelpCheck(const Flags &flags) -> bool
{
	if(!flags.Contains(kHelpFlag))
		return false;

	std::cout << "Help!\n";

	return true;
}