#pragma once
#include <Windows.h>

class FpsControl
{
	public:
	//LIFECYCLE
		FpsControl(FpsControl&);


	//ACCESS
		static FpsControl& GetInstance();

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
		void Draw();

	//VARIABLES
		DWORD startTime, startCountingTime;
		int passTime, waitTime; // �����[�v�������ԁi�o�͗p�ɕۑ��j
		int count;
		double fps;
		std::string strFps;
};