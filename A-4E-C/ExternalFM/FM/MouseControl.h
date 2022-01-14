#pragma once
#include <math.h>
#include <Windows.h>
#include <limits.h>

constexpr double c_inputScale = 10.0;

class MouseAxis
{
public:

	int update( double input )
	{
		input *= c_inputScale;

		if ( abs(input) > 1.0 )
			return (int)input;

		int frequency = input == 0.0 ? INT_MAX : 1 + (double)(1.0 / abs(input));

		if ( frequency < m_timer || m_timer == 0 )
		{
			m_timer = frequency;
			double sign = copysign( 1.0, input );
			m_sign = sign;
		}

		m_timer--;
		if ( m_timer )
			return 0;

		return m_sign;
	}



private:
	int m_sign = 0;
	int m_timer = 0;
};



class MouseControl
{
public:

	void update(double mouseX, double mouseY)
	{
		int dx = m_x.update( mouseX );
		int dy = m_y.update( mouseY );

		if ( dx || dy )
		{
			INPUT buffer;
			memset( &buffer, 0, sizeof( INPUT ) );
			buffer.type = INPUT_MOUSE;
			buffer.mi.dx = dx;
			buffer.mi.dy = dy;
			buffer.mi.mouseData = 0;
			buffer.mi.dwFlags = MOUSEEVENTF_MOVE;
			buffer.mi.time = 0;
			buffer.mi.dwExtraInfo = 0;
			SendInput( 1, &buffer, sizeof( INPUT ) );
		}
	}

private:

	MouseAxis m_x;
	MouseAxis m_y;
};