#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <Windows.h>

#include "output_data.h"

class OutputWindow : public Command
{
public:
	//LIFECYCLE
	virtual ~OutputWindow();

	//OPRATOR
	template<class T>
	OutputWindow& operator<<(T val);

	//OPERATIONS
	void Init();
	void Print(std::string);


protected:
	static const std::string dispConsole;
	static const WORD colors[];

	//このクラスの生成禁止
	OutputWindow(DWORD, DWORD, std::string, std::string, DWORD = 1);

	//VARIABLES
	std::string commandLine;
	HANDLE readPipe;
	DWORD procGroupId;


private:
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hProcThread;
	HANDLE hParenProcess;  // カレントプロセスのハンドル
	HANDLE hChildProcess;  // 子プロセスのハンドル
	HANDLE writePipe;
	std::string title;

	//OPERATIONS
	bool StartProcess();
	static unsigned int __stdcall LaunchThread(void *);
	unsigned int __stdcall TProcess();

};


template<class T>
inline OutputWindow& OutputWindow::operator<<(T val)
{
	std::stringstream buf;
	buf << val << '|';
	Print( buf.str() );

	return *this;
}