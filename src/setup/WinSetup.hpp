#ifndef WIN_SETUP_HPP
#define WIN_SETUP_HPP

class Flags;

void RemoveFiles();
void RemoveRegistry();
void RemoveTask(const Flags &flags);

void Setup(const Flags &flags);
void CheckIntegrity();
void SetupRegistry();
void InitialGenerate(const Flags &flags);
void AddTaskToScheduler(const Flags &flags);

#endif // WIN_SETUP_HPP