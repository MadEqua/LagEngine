#pragma once

namespace Lag 
{
	class ICursorListener
	{		
	public:
		virtual ~ICursorListener() = default;

		virtual void onCursorMove(int x, int y) = 0;
		virtual void onButtonPressed(int x, int y, int button, int modifiers) = 0;
		virtual void onButtonReleased(int x, int y, int button, int modifiers) = 0;
	};
}