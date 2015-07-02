#pragma once
#include <Windows.h>

class FpsControl
{
	public:
	//LIFECYCLE
		FpsControl(FpsControl&);

	//ACCESS
		static FpsControl& GetInstance ();
		double PassTime() const { return passTime; }
		double WaitTime() const { return waitTime; }
		std::string GetInfo() const;

	//OPERRATIONS
		void SetStartTime();
		void Update();
		void Wait();


	private:
	//CONSTANTS
		static const int MAX_COUNT = 10;
		static const int STANDERD_FPS = 30;

	//LIFECYCLE
		FpsControl();
		virtual ~FpsControl() = default;

	//OPERATIONS
		void CalcFps();

	//VARIABLES
		DWORD startTime, startCountingTime;
		int passTime, waitTime; // 現ループ処理時間（出力用に保存）
		int count;
		double fps;
		std::string strFps;
};