#include "Logger.h"
#include <string>

namespace Logger
{
	static void(*cout_func)(LogLevel, std::string) = nullptr;

	void Log(LogLevel level, std::string message)
	{
		if (cout_func)
			cout_func(level, message);
	}

	void Bind(void (*func)(LogLevel, std::string))
	{
		cout_func = func;
	}
}
