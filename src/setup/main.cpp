#include <iostream>

#include "Help.hpp"
#include "FlagParser.hpp"
#include "Clear.hpp"
#include "Setup.hpp"
#include "ScriptManager.hpp"
#include "Api.hpp"


int main(int argc, const char **argv)
{
    const char **utf8_args = ConvertArgsToUtf8(argc, argv);
	const Flags flags(argc, utf8_args);
	ScriptManager script_manager;
	
	if(script_manager.Check(flags))
		return 0;
	if(HelpCheck(flags))
		return 0;
	if(ClearCheck(flags))
		return 0;

	Setup(flags);

	for(int i = 0; i < argc; i++)
        delete[] utf8_args[i];
    delete[] utf8_args;

	return 0;
}