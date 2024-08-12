#include <iostream>

#include "Help.hpp"
#include "FlagParser.hpp"
#include "Clear.hpp"
#include "Setup.hpp"


int main(int argc, const char **argv)
{
	const Flags flags(argc, argv);
	
	if(HelpCheck(flags))
		return 0;
	if(ClearCheck(flags))
		return 0;

	Setup(flags);

	return 0;
}