#pragma once

#include <chrono>

#include "../Types.h"

namespace Lag
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		void start();
		float getElapsedSeconds();
		uint32 getElapsedMilis();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
	};
}