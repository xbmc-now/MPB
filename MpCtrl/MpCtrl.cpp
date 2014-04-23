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

