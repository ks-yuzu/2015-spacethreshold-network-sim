#pragma once

class Pos
{
	public:
		Pos(int ax, int ay) : x(ax), y(ay){}
		Pos(const Pos& p) : x(p.x), y(p.y){}

		const Pos& operator+=(const Pos& ap){ x += ap.x;  y += ap.y;  return *this; }
		const Pos& operator*=(const int a){ x *= a;  y *= a;  return *this; }

		int x, y;

	private:
		Pos(){}
};