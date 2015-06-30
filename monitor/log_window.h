#pragma once
#pragma warning(disable : 4996)

#include <chrono>
#include <iomanip>
#include <iostream>

#include "output_window.h"
#include "output_data.h"


//---------------  HOW TO INIT  ---------------
//	Log::cout.Init();
//	Log::cout << "Log Window" << Command::endline << Command::endline;
//---------------------------------------------

class Log : public OutputWindow
{
public:
	Log();
	virtual ~Log() = default;

	template<class T> Log& operator<<(T val);

	void Init();
	static Log& lout(){ return instance; };


private:
	static Log instance;
	static const std::string logConsole;

	bool fDispTime;
	std::stringstream buf;

	void PrintTime();
	template<class T> void PrintData(T);
};




template<typename T>
inline Log& Log::operator<<(T val)
{
	if( fDispTime )
		PrintTime();

	PrintData(val);

	return *this;
}

//“ÁŽê‰»
template<> 
inline Log& Log::operator<< <std::string>(std::string val)
{
	if( fDispTime && val != Command::endline )
		PrintTime();

	PrintData(val);

	if( val == Command::endline )
	{
		Print(buf.str());
		buf.str("");
		buf.clear(std::stringstream::goodbit);
		fDispTime = true;
	}

	return *this;
}



template<class T>
inline void Log::PrintData(T val)
{
	buf << val;
}


