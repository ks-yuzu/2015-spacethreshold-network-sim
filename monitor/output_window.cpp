#pragma warning(disable : 4996)

#include <iostream>
#include <string>
#include <cassert>
#include <process.h>
#include "output_window.h"
#include "output_data.h"


const std::string OutputWindow::dispConsole = "OutputWindow.exe /D";

const WORD OutputWindow::colors[] = {
		FOREGROUND_RED   | FOREGROUND_INTENSITY,						//DUMMY
		FOREGROUND_GREEN | FOREGROUND_INTENSITY,						//Green
		FOREGROUND_BLUE  | FOREGROUND_RED   |  FOREGROUND_INTENSITY,	//Purple
		FOREGROUND_BLUE  | FOREGROUND_GREEN |  FOREGROUND_INTENSITY,	//Cyan
		FOREGROUND_GREEN | FOREGROUND_RED   |  FOREGROUND_INTENSITY,	//Yellow
		FOREGROUND_BLUE  | FOREGROUND_INTENSITY,						//Blue
		FOREGROUND_RED   | FOREGROUND_INTENSITY,						//Red
};


OutputWindow::OutputWindow(DWORD posX, DWORD posY, std::string wndTitle, std::string command, DWORD colorID)
:hParenProcess( GetCurrentProcess() ),
 hChildProcess(NULL),
 readPipe(nullptr),
 writePipe(nullptr),
 commandLine(command),
 title(wndTitle)
{
	ZeroMemory( &si, sizeof(STARTUPINFO) );

	si.cb				= sizeof(STARTUPINFO);
	si.dwFlags			= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW | STARTF_USEPOSITION | STARTF_USEFILLATTRIBUTE;
	si.wShowWindow		= SW_SHOWNOACTIVATE;
	si.lpTitle			= const_cast<LPSTR>( title.c_str() );
	si.dwX				= posX;
	si.dwY				= posY;
	si.dwFillAttribute	= colors[colorID];
}


OutputWindow::~OutputWindow()
{
//	GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, procGroupId);
	TerminateProcess(hChildProcess, EXIT_SUCCESS);
	CloseHandle(writePipe);
	CloseHandle(hChildProcess);
}


void OutputWindow::Init()
{
	HANDLE tmpReadPipe;

	// パイプを作成（両ハンドルとも子プロセスへ継承不可）
	assert( CreatePipe(&tmpReadPipe, &writePipe, NULL, 0) );

	// 読込ハンドルを複製（子プロセスへ継承可能な権限の読込ハンドルを作成）
	assert( DuplicateHandle( hParenProcess, tmpReadPipe, hParenProcess, &readPipe, 0, TRUE, DUPLICATE_SAME_ACCESS) );

	// 複製元のハンドルは使わないのでクローズ
	assert( CloseHandle(tmpReadPipe) );

	// 確保した標準出力のチェック
	assert( si.hStdOutput != INVALID_HANDLE_VALUE );
	assert( si.hStdError != INVALID_HANDLE_VALUE );

	si.hStdInput		= readPipe;
	si.hStdOutput		= GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError		= GetStdHandle(STD_ERROR_HANDLE);

	StartProcess();
}


bool OutputWindow::StartProcess()
{
	hProcThread = (HANDLE)_beginthreadex(NULL, 0, LaunchThread, this, 0, NULL);

	if( hProcThread != nullptr )
		CloseHandle( hProcThread );

	return true;
}


void OutputWindow::Print(std::string str)
{
	DWORD dummy;
	str += '|';

	char const *mes = str.c_str();
	WriteFile(writePipe, mes, strlen(mes), &dummy, NULL);
}



//=======================================
//           子プロセス側
//=======================================

// (非static)メンバ関数をマルチスレッドにするために、この関数をかませる
unsigned int __stdcall OutputWindow::LaunchThread(void *ptr)
{
	return reinterpret_cast<OutputWindow*>(ptr)->TProcess();
}


unsigned int __stdcall OutputWindow::TProcess()
{
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	assert( CreateProcess(NULL, (LPSTR)commandLine.c_str(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi) );
//	procGroupId = pi.dwProcessId;

	// Set child process handle to cause threads to exit.
	hChildProcess = pi.hProcess;

	// Close any unnecessary handles.
	CloseHandle(pi.hThread);

	// Close pipe handles (do not continue to modify the parent)
	// Need to make sure that no handles to the write end of the output pipe are maintained in this process
	// or else the pipe will not close when the child process exits
	// and the ReadFile will hang.
	CloseHandle(readPipe);
	readPipe = nullptr;

	return 0;

}