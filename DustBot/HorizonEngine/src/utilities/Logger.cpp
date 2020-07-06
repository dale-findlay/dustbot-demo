#include "Logger.h"

#include <iostream>
#include <sstream>
#include <mutex>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

Horizon::Logger::Logger()
{}

bool Horizon::Logger::Init()
{
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	return true;
}

void Horizon::Logger::Shutdown()
{

}

std::string Horizon::Logger::GetSystemTime()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);

	std::stringstream ss;
	ss << st.wHour << ":" << st.wMinute << ":" << st.wSecond << ":" << st.wMilliseconds;

	return ss.str();
}

void Horizon::Logger::Log(const std::string& message, bool newLine)
{
	m_mutex.lock();
	SetConsoleColor(10);
	if (newLine)
	{
		std::cout << GetSystemTime() << " ";
	}
	SetConsoleColor(15);
	std::cout << message;

	if (newLine)
	{
		std::cout << std::endl;
	}
	m_mutex.unlock();
}

void Horizon::Logger::LogWarning(const std::string& message, bool newLine)
{
	m_mutex.lock();
	SetConsoleColor(14);
	if (newLine)
	{
		std::cout << GetSystemTime() << " ";
	}
	SetConsoleColor(15);
	std::cout << message;

	if (newLine)
	{
		std::cout << std::endl;
	}
	m_mutex.unlock();
}

void Horizon::Logger::LogError(const std::string& message, bool newLine)
{
	m_mutex.lock();
	SetConsoleColor(12);
	if (newLine)
	{
		std::cout << GetSystemTime() << " ";
	}
	SetConsoleColor(15);
	std::cout << message;

	if (newLine)
	{
		std::cout << std::endl;
	}
	m_mutex.unlock();
}

void Horizon::Logger::SetConsoleColor(const int color)
{
	SetConsoleTextAttribute(m_hConsole, color);
}
