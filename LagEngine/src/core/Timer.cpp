#include "Timer.h"

using namespace Lag;

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::start()
{
	startPoint = std::chrono::high_resolution_clock::now();
}

float Timer::getElapsedSeconds()
{
	std::chrono::duration<float> dif = std::chrono::high_resolution_clock::now() - startPoint;
	uint64 ms = std::chrono::duration_cast<std::chrono::milliseconds>(dif).count();
	return static_cast<float>(ms) / 1000.0f;
}

uint32 Timer::getElapsedMilis()
{
	std::chrono::duration<float> dif = std::chrono::high_resolution_clock::now() - startPoint;
	uint64 ms = std::chrono::duration_cast<std::chrono::milliseconds>(dif).count();
	return static_cast<uint32>(ms);
}