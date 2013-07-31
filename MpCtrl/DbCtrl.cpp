#include "stdafx.h"
#include "DbCtrl.h"
#include "DbCtrlDef.h"
#include <process.h>

CDbCtrl::CDbCtrl(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);
	this->buffLockEvent = _CreateEvent(FALSE, TRUE, NULL);
}
CDbCtrl::~CDbCtrl(void)
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

BOOL CDbCtrl::Lock(LPCWSTR log, DWORD timeOut)
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
				_OutputDebugString(L"��CDbCtrl::Lock FALSE : %s", log);
			}else{
				OutputDebugString(L"��CDbCtrl::Lock FALSE");
			}
		return FALSE;
	}
	return TRUE;
}

void CDbCtrl::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

void CDbCtrl::GetMpLogPath(CString& szValue)
{
	szValue.GetEnvironmentVariable(L"ALLUSERSPROFILE");
	szValue += L"\\Team MediaPortal\\MediaPortal TV Server\\log\\tv.log";
}
DWORD CDbCtrl::GetMpServiceStatus()
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

