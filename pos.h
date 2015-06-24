#pragma once

#include "monitor/monitor_window.h"

class Pos
{
	public:
		// friends
		friend std::ostream& operator<<(std::ostream& os, const Pos& pos);
		friend Monitor& operator<<(Monitor& ms, const Pos& pos);

		// lifecycle
		Pos(int ax, int ay) : x(ax), y(ay){}
		Pos(const Pos& p) : x(p.x), y(p.y){}

		// operator
		const Pos& operator+=(const Pos& ap){ x += ap.x;  y += ap.y;  return *this; }
		const Pos& operator-=(const Pos& ap){ x -= ap.x;  y -= ap.y;  return *this; }
		const Pos& operator*=(const int a){ x *= a;  y *= a;  return *this; }
		const Pos& operator/=(const double a){ x = static_cast<int>(x / a);  y = static_cast<int>(x / a);  return *this; }

		// variable
		int x, y;

	private:
		Pos(){}
};


inline std::ostream& operator<<(std::ostream& os, const Pos& pos)
{
    os << "(" << pos.x << ", " << pos.y << ")";
    return os;
}

inline Monitor& operator<<(Monitor& ms, const Pos& pos)
{
    ms << "(" << pos.x << ", " << pos.y << ")";
    return ms;
}