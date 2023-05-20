#pragma once

#include <iostream>
#include <fmt/core.h>

#ifdef _DEBUG
	#define LOG_INFO(msg, ...)  Logger::Info(msg, ##__VA_ARGS__)
	#define LOG_WARN(msg, ...)  Logger::Warn(msg, ##__VA_ARGS__)
	#define LOG_ERROR(msg, ...) Logger::Error(msg, ##__VA_ARGS__)
	#define LOG_DEBUG(msg, ...) Logger::Debug(msg, ##__VA_ARGS__)
#else
	#define LOG_INFO(msg, ...)
	#define LOG_WARN(msg, ...)
	#define LOG_ERROR(msg, ...)
	#define LOG_DEBUG(msg, ...)
#endif


#define CONSOLE_WHITE "\e[0;37m"
#define CONSOLE_GREEN "\e[0;32m"
#define CONSOLE_YELLOW "\e[0;33m"
#define CONSOLE_RED "\e[0;31m"


namespace Logger
{
	enum LogLevel
	{
		LOG_LEVEL_DEBUG,
		LOG_LEVEL_INFO,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_ERROR
	};

	void Log(LogLevel level, std::string message);
	void Bind(void(*func)(LogLevel, std::string));

	template <typename... T>
	void Debug(std::string message, T&&... args)
	{
		std::string final_message = fmt::format(message, args...);
		std::cout << "[DEBUG]: " << final_message << std::endl;
		Log(LogLevel::LOG_LEVEL_DEBUG, final_message);
	}
    
	template <typename... T>
	void Info(std::string message, T&&... args)
	{
		std::string final_message = fmt::format(message, args...);
		std::cout << CONSOLE_GREEN << "[INFO]: " << final_message << CONSOLE_WHITE << std::endl;
		Log(LogLevel::LOG_LEVEL_INFO, final_message);
	}
    
	template <typename... T>
	void Warn(std::string message, T&&... args)
	{
		std::string final_message = fmt::format(message, args...);
		std::cout << CONSOLE_YELLOW << "[WARNING]: " << final_message << CONSOLE_WHITE << std::endl;
		Log(LogLevel::LOG_LEVEL_WARNING, final_message);
	}

	template <typename... T>
	void Error(std::string message, T&&... args)
	{
		std::string final_message = fmt::format(message, args...);
		std::cout << CONSOLE_RED << "[ERROR]: " << final_message << CONSOLE_WHITE << std::endl;
		Log(LogLevel::LOG_LEVEL_ERROR, final_message);
	}
} // namespace Logger
