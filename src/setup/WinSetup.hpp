#ifndef WIN_SETUP_HPP
#define WIN_SETUP_HPP

class Flags;

void Setup(const Flags &flags);
void CheckIntegrity();
void CreateBat();
void SetupRegistry();
void InitialGenerate(const Flags &flags);
void AddTaskToScheduler(const Flags &flags);
void TerminateAdministrator();

#endif // WIN_SETUP_HPP