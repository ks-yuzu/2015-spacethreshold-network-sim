#pragma once

class Pos
{
	public:
		Pos(int ax, int ay) : x(ax), y(ay){}
		Pos(const Pos& p) : x(p.x), y(p.y){}
		int x, y;

	private:
		Pos(){}
};