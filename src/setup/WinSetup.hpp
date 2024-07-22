#ifndef WIN_SETUP_HPP
#define WIN_SETUP_HPP

void Setup(const char **args_begin, const char **args_end);
void SetupRegistry();
void InitialGenerate(const char **args_begin, const char **args_end);
void TerminateRegistry();

#endif // WIN_SETUP_HPP