#pragma once

namespace Lag
{
	class IKeyboardListener
	{
	public:
		virtual ~IKeyboardListener() = default;

		virtual void onKeyPress(int key, int modifier) = 0;
		virtual void onKeyRelease(int key, int modifier) = 0;
		virtual void onKeyRepeat(int key, int modifier) = 0;
	};
}