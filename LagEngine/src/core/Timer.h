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
		float getElapsedSeconds() const;
		uint32 getElapsedMilis() const;

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
	};
}