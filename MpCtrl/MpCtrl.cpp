#include "stdafx.h"
#include "MpCtrl.h"
#include <process.h>

CMpCtrl::CMpCtrl(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);
	this->buffLockEvent = _CreateEvent(FALSE, TRUE, NULL);
}
CMpCtrl::~CMpCtrl(void)
{
	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
	if( this->buffLockEvent != NULL ){
		CloseHandle(this->buffLockEvent);
		this->buffLockEvent = NULL;
	}

}

BOOL CMpCtrl::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	//if( log != NULL ){
	//	OutputDebugString(log);
	//}
	DWORD dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED ||
		dwRet == WAIT_TIMEOUT){
			if( log != NULL ){
				_OutputDebugString(L"◆CMpCtrl::Lock FALSE : %s", log);
			}else{
				OutputDebugString(L"◆CMpCtrl::Lock FALSE");
			}
		return FALSE;
	}
	return TRUE;
}

void CMpCtrl::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

void CMpCtrl::GetMpLogPath(CString& szValue)
{
	szValue.GetEnvironmentVariable(_T("ALLUSERSPROFILE"));
	szValue += _T("\\Team MediaPortal\\MediaPortal TV Server\\log\\TVService.log");
}
DWORD CMpCtrl::GetMpServiceStatus()
{
	// サービス制御マネージャを起動する。
	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_CONNECT );
	if( hSCManager == NULL ) return 1; // サービスマネージャが起動ができませんでした。

	SERVICE_STATUS ssSTAT; //  サービス状態構造体

	// サービスに接続する
	SC_HANDLE hSVC = OpenService(hSCManager,_T("TVService"),SERVICE_QUERY_STATUS);
	if (hSVC == NULL) return 2; // サービスに接続ができませんでした。

	QueryServiceStatus(hSVC,&ssSTAT);
	if (ssSTAT.dwCurrentState != SERVICE_RUNNING) return 3; // サービスが起動していません
	return 0;
}

DWORD CMpCtrl::RestartMpService()
{
	// サービス制御マネージャを起動する。
	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_CONNECT );
	if( hSCManager == NULL ) return 1; // サービスマネージャが起動ができませんでした。

	// サービスに接続する
	SC_HANDLE hSVC = OpenService(hSCManager,_T("TVService"),SERVICE_QUERY_STATUS);
	if (hSVC == NULL) return 2; // サービスに接続ができませんでした。

	DWORD result;
	result = StopThenStartService(hSVC, _T("TVService"));
	(void) CloseServiceHandle(hSVC);
	return 0;
}

// 指定されたサービスを停止させ、起動する。
// manager : サービス マネージャ
// serviceName : サービス名
DWORD CMpCtrl::StopThenStartService(SC_HANDLE manager, LPCTSTR serviceName)
{
	if (manager == NULL || serviceName == NULL) { return ERROR_BAD_ARGUMENTS; }

	SC_HANDLE	 service;
	service = OpenService(manager, serviceName, GENERIC_EXECUTE | GENERIC_READ);

	if (service != NULL) {
		DWORD result;
		// サービスを停止する
		if ((result = StopServiceWithWaiting(service)) == ERROR_SUCCESS) {
			// サービスを起動する
			result = StartServiceWithWaiting(service);
		}
		(void) CloseServiceHandle(service);
		return result;

	} else {
		return GetLastError();
	}
}

// 指定のサービスを停止させ、停止するまで待つ。
// service : サービスのハンドラ
DWORD CMpCtrl::StopServiceWithWaiting(SC_HANDLE service)
{
	if (service == NULL) { return ERROR_BAD_ARGUMENTS; }

	SERVICE_STATUS status;
	BOOL ret = ControlService(service, SERVICE_CONTROL_STOP, &status);
	DWORD result = GetLastError();
	if (ret == TRUE || result == ERROR_SERVICE_NOT_ACTIVE) {
		result = WaitServiceUntilStatus(service, SERVICE_STOPPED);
	}
	return result;
}

// 指定のサービスを起動させ、起動するまで待つ。
// service : サービスのハンドラ
DWORD CMpCtrl::StartServiceWithWaiting(SC_HANDLE service)
{
	if (service == NULL) { return ERROR_BAD_ARGUMENTS; }

	BOOL ret = StartService(service, 0, NULL);
	DWORD result = GetLastError();
	if (ret == TRUE || result == ERROR_SERVICE_ALREADY_RUNNING) {
		result = WaitServiceUntilStatus(service, SERVICE_RUNNING);
	}
	return result;
}

// サービスが、指定の状態になるまで待つ。
// service : サービスのハンドラ
// state : 期待する状態
// timeout : 期待する状態になるまで待機する秒数（既定値 30）
DWORD CMpCtrl::WaitServiceUntilStatus(SC_HANDLE service, DWORD state, int timeout)
{
	DWORD result;
	SERVICE_STATUS status;
	int count = 0;
	while (TRUE) {
		if (QueryServiceStatus(service, &status)) {
			if (status.dwCurrentState == state) {
				result = ERROR_SUCCESS;
				break;
			}
			if (++count > timeout) {
				result = ERROR_SERVICE_REQUEST_TIMEOUT;
				break;
			}
		} else {
			result = GetLastError();
			break;
		}
		Sleep(1000);
	}
	return result;
}
