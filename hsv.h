#include <cassert>

struct Hsvd
{
	Hsvd(int h, double s, double v) : h(h), s(s), v(v) {}
	int h;
	double s, v;
};

struct Rgbd
{
	Rgbd(double r, double g, double b) : r(r), g(g), b(b) {}
	double r, g, b;
};

static Rgbd Hsvd2Rgbd(int h, double s, double v)
{
	if( s == 0 )  { return Rgbd(v, v, v); };

	int hi = int(h / 60) % 6;
	double f = (double)h / 60 - hi;

	double i = v * (1 - s);
	double j = v * (1 - f * s);
	double k = v * (1 - s * (1 - f));

	switch (hi)
	{
		case 0:  return Rgbd(v, k, i);
		case 1:  return Rgbd(j, v, i);
		case 2:  return Rgbd(i, v, k);
		case 3:  return Rgbd(i, j, v);
		case 4:  return Rgbd(k, i, v);
		case 5:  return Rgbd(v, i, j); 
		default: assert(0);
	}

	 return Rgbd(0, 0, 0);
}