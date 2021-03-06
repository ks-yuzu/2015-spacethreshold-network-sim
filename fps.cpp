#include <iostream>
#include <sstream>
#include <iomanip>
#include "fps.h"
#include "monitor/monitor_window.h"

#pragma comment(lib, "winmm.lib")


FpsControl::FpsControl()
:startTime(timeGetTime()), count(0), fps(0)
{}


FpsControl::FpsControl(FpsControl& aFps)
:startTime(aFps.startTime), count(aFps.count), fps(aFps.fps)
{}


FpsControl& FpsControl::GetInstance()
{
	static FpsControl fps;
	return fps;
}


void FpsControl::SetStartTime()
{
	startTime = timeGetTime();
//	Draw();
}

void FpsControl::Update()
{
	count++;

	if( count == MAX_COUNT )
	{
		CalcFps();
		count = 0;
		startCountingTime = timeGetTime();
	}
}


void FpsControl::CalcFps()
{
	int now = timeGetTime();
	fps = 1000. * MAX_COUNT / ( now - startCountingTime );

	std::ostringstream buf;
	buf << std::fixed << std::setprecision(2) << fps;
	strFps = buf.str();
}

std::string FpsControl::GetInfo() const
{
	std::ostringstream buf;
	buf << "[FPS]" << Command::endline
		<< "  FPS      : " << strFps << Command::endline
		<< "  passTime : " << std::setw(4) << std::right << passTime << " ms" << Command::endline
	    << "  waitTime : " << std::setw(4) << std::right << waitTime << " ms" << Command::endline;

	return buf.str();
}


void FpsControl::Wait()
{
	// 経過時間及びFPS調整待ち時間計算
	passTime = timeGetTime() - startTime;
	waitTime = (int)( 1000. / STANDERD_FPS - passTime );

	if( waitTime > 0 ) { Sleep( waitTime ); }
	else               { waitTime = 0; /*出力用修正*/ }
}