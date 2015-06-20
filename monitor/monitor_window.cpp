#include <cassert>
#include "monitor_window.h"


std::vector<std::shared_ptr<Monitor> > Monitor::outputs(6);

Monitor::Monitor(int wn)
:windowNum(wn+1),
OutputWindow(
	1010 + 650 * (windowNum / 2),
	20 + 494 * (windowNum % 2),
	"OutputWindow No." + std::to_string(windowNum),
	dispConsole,
	windowNum
)
{}
