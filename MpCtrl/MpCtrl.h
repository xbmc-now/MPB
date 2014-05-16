#pragma once

#include <windows.h>
#include <winsvc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "../Common/Util.h"


class CMpCtrl
{
public:
	CMpCtrl(void);
	~CMpCtrl(void);
	
	void GetMpLogPath(CString& szValue);
	DWORD GetMpServiceStatus();
	DWORD RestartMpService();

	DWORD StopThenStartService(SC_HANDLE manager, LPCTSTR serviceName);
	DWORD StopServiceWithWaiting(SC_HANDLE service);
	DWORD StartServiceWithWaiting(SC_HANDLE service);
	DWORD WaitServiceUntilStatus(SC_HANDLE service, DWORD state, int timeout = 30);

protected:

	HANDLE lockEvent;

	HANDLE buffLockEvent;


protected:
	//PublicAPIîrëºêßå‰óp
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 15*1000);
	void UnLock(LPCWSTR log = NULL);


};

