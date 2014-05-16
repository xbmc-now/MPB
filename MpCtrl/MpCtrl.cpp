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
				_OutputDebugString(L"��CMpCtrl::Lock FALSE : %s", log);
			}else{
				OutputDebugString(L"��CMpCtrl::Lock FALSE");
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
	// �T�[�r�X����}�l�[�W�����N������B
	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_CONNECT );
	if( hSCManager == NULL ) return 1; // �T�[�r�X�}�l�[�W�����N�����ł��܂���ł����B

	SERVICE_STATUS ssSTAT; //  �T�[�r�X��ԍ\����

	// �T�[�r�X�ɐڑ�����
	SC_HANDLE hSVC = OpenService(hSCManager,_T("TVService"),SERVICE_QUERY_STATUS);
	if (hSVC == NULL) return 2; // �T�[�r�X�ɐڑ����ł��܂���ł����B

	QueryServiceStatus(hSVC,&ssSTAT);
	if (ssSTAT.dwCurrentState != SERVICE_RUNNING) return 3; // �T�[�r�X���N�����Ă��܂���
	return 0;
}

DWORD CMpCtrl::RestartMpService()
{
	// �T�[�r�X����}�l�[�W�����N������B
	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_CONNECT );
	if( hSCManager == NULL ) return 1; // �T�[�r�X�}�l�[�W�����N�����ł��܂���ł����B

	// �T�[�r�X�ɐڑ�����
	SC_HANDLE hSVC = OpenService(hSCManager,_T("TVService"),SERVICE_QUERY_STATUS);
	if (hSVC == NULL) return 2; // �T�[�r�X�ɐڑ����ł��܂���ł����B

	DWORD result;
	result = StopThenStartService(hSVC, _T("TVService"));
	(void) CloseServiceHandle(hSVC);
	return 0;
}

// �w�肳�ꂽ�T�[�r�X���~�����A�N������B
// manager : �T�[�r�X �}�l�[�W��
// serviceName : �T�[�r�X��
DWORD CMpCtrl::StopThenStartService(SC_HANDLE manager, LPCTSTR serviceName)
{
	if (manager == NULL || serviceName == NULL) { return ERROR_BAD_ARGUMENTS; }

	SC_HANDLE	 service;
	service = OpenService(manager, serviceName, GENERIC_EXECUTE | GENERIC_READ);

	if (service != NULL) {
		DWORD result;
		// �T�[�r�X���~����
		if ((result = StopServiceWithWaiting(service)) == ERROR_SUCCESS) {
			// �T�[�r�X���N������
			result = StartServiceWithWaiting(service);
		}
		(void) CloseServiceHandle(service);
		return result;

	} else {
		return GetLastError();
	}
}

// �w��̃T�[�r�X���~�����A��~����܂ő҂B
// service : �T�[�r�X�̃n���h��
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

// �w��̃T�[�r�X���N�������A�N������܂ő҂B
// service : �T�[�r�X�̃n���h��
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

// �T�[�r�X���A�w��̏�ԂɂȂ�܂ő҂B
// service : �T�[�r�X�̃n���h��
// state : ���҂�����
// timeout : ���҂����ԂɂȂ�܂őҋ@����b���i����l 30�j
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
