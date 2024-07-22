#ifndef WIN_API_HPP
#define WIN_API_HPP

// Start process specified by path, with arguments and wait till it finished
void ExecuteProgram(const char *program_path, char *args);
void TerminateExecute();
void GetScreenResolution(int &width, int &height);
void MySleep(int time_seconds);

#endif // WIN_API_HPP