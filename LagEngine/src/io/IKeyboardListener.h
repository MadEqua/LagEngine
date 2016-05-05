#pragma once

#include "../Types.h"

namespace Lag
{
	class IKeyboardListener
	{
	public:
		virtual ~IKeyboardListener() {}

		virtual void onKeyPress(int key, int modifier) = 0;
		virtual void onKeyRelease(int key, int modifier) = 0;
		virtual void onKeyRepeat(int key, int modifier) = 0;
	};
}