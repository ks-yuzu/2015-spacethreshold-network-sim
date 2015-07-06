#pragma once

#include "define.h"
#include "monitor/monitor_window.h"

#include <climits>

class Pos
{
	public:
		// friends
		friend std::ostream& operator<<(std::ostream& os, const Pos& pos);
		friend Monitor& operator<<(Monitor& ms, const Pos& pos);

		// lifecycle
		constexpr Pos(int ax, int ay) : x(ax), y(ay) {}
//		Pos(int ax, int ay) : x(ax), y(ay){}
		Pos(const Pos& p) = default;
		~Pos() = default;

		// operator
		const Pos& operator+=(const Pos& ap) { x += ap.x;  y += ap.y;  return *this; }
		const Pos& operator-=(const Pos& ap) { x -= ap.x;  y -= ap.y;  return *this; }
		const Pos& operator*=(const int a)   { x *= a;  y *= a;  return *this; }
		const Pos& operator/=(const double a){ x = static_cast<int>(x / a);  y = static_cast<int>(x / a);  return *this; }
		bool operator==(const Pos& a) const { return x == a.x && y == a.y; }

		// operation
		static long long int distsq(const Pos& p1, const Pos& p2)
		{
			// 2���long long int�ŕԂ������_
			auto pow2li = [](int a){return (__int64)a * a;};
			return pow2li(p1.x - p2.x) + pow2li(p1.y - p2.y);
		}

		// variable
		int x, y;

	private:
		Pos() = default;
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