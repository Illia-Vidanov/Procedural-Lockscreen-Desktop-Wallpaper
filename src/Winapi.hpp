#ifndef WIN_API_HPP
#define WIN_API_HPP

// Start process specified by path, with arguments and wait till it finished, exit code is returned
int ExecuteProgram(const char *program_path, char *args);
void TerminateAdmin();
void GetScreenResolution(int &width, int &height);

#endif // WIN_API_HPP