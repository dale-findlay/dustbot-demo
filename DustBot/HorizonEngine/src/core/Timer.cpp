#include "Timer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool Horizon::Timer::Init()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	m_frequency = (double)frequency.QuadPart;
	m_secsPerCount = 1.0 / m_frequency;

	return true;
}

void Horizon::Timer::Shutdown()
{}

void Horizon::Timer::PreUpdateTimer()
{
	LARGE_INTEGER preFrameCount;
	QueryPerformanceCounter(&preFrameCount);

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	m_frequency = (double)frequency.QuadPart;
	m_secsPerCount = 1.0 / m_frequency;

	m_preFrameCount = preFrameCount.QuadPart;
}

void Horizon::Timer::PostUpdateTimer()
{
	LARGE_INTEGER postFrameCount;
	QueryPerformanceCounter(&postFrameCount);

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	m_frequency = (double)frequency.QuadPart;
	m_secsPerCount = 1.0 / m_frequency;

	m_postFrameCount = postFrameCount.QuadPart;
}

Horizon::float32 Horizon::Timer::CalculateDeltaTime()
{
	int countDiff = (int)(m_postFrameCount - m_preFrameCount);
	return (float32) (countDiff * m_secsPerCount);
}

Horizon::uint32 Horizon::Timer::CalculateFramesPerSecond()
{
	return (uint32)(1.0f / CalculateDeltaTime());
}
