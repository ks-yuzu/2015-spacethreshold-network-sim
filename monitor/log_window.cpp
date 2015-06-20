#include <cassert>
#include "log_window.h"


// instance of static val
const std::string Log::logConsole  = "OutputWindow.exe /L";
Log Log::instance;


Log::Log()
:OutputWindow(10, 20, "LogWindow", /*"OutputWindow.exe /L"*/logConsole, 5),
fDispTime(false)
{
}

void Log::Init()
{
	//ログウィンドウの設定とタイトル
	this->OutputWindow::Init();
	*this << "Log Window" << Command::endline << Command::endline;
}


void Log::PrintTime()
{
	using namespace std::chrono;

	auto tp = system_clock::now();
	std::time_t now = system_clock::to_time_t(tp);
	buf << std::put_time(std::localtime(&now), "%X") << "  ";

	fDispTime = false;
}