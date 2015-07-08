#include <sstream>

std::string MakeProgBar(double rate, int len = 25)
{
	std::stringstream ss;
	ss << '[';
	for (int i = 0; i < len; ++i) { ss << (i <= len * rate ? '*' : ' '); }
	ss << ']';
	return ss.str();
}
