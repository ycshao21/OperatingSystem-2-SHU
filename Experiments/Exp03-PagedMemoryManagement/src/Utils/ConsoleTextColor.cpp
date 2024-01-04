#include "ConsoleTextColor.h"

#include <Windows.h>

void SetConsoleTextColor(ConsoleTextColor color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(color));
}