#pragma once

#include "StructDef.h"

typedef int (CALLBACK *CMD_CALLBACK_PROC)(void* pParam, CMD_STREAM* pCmdParam, CMD_STREAM* pResParam);

class CPipeServer
{
public:
	CPipeServer(void);
	~CPipeServer(void);

	BOOL StartServer(
		LPCWSTR eventName, 
		LPCWSTR pipeName, 
		CMD_CALLBACK_PROC cmdCallback, 
		void* callbackParam, 
		int threadPriority = THREAD_PRIORITY_NORMAL,
		int ctrlCmdEventID = -1
		);
	void StopServer();

protected:
	CMD_CALLBACK_PROC cmdProc;
	void* cmdParam;
	wstring eventName;
	wstring pipeName;

	int threadPriority;
	int ctrlCmdEventID;

	HANDLE stopEvent;
	HANDLE workThread;

protected:
	static UINT WINAPI ServerThread(LPVOID pParam);

};
