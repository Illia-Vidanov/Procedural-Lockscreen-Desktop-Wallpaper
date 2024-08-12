#ifndef CLEAR_HPP
#define CLEAR_HPP

class Flags;

auto ClearCheck(const Flags &flags) -> bool;

void RemoveFiles();
void RemoveRegistry();
void RemoveTask(const Flags &flags);

#endif // CLEAR_HPP