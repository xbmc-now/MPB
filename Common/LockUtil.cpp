#include "StdAfx.h"
#include "LockUtil.h"

#include "Util.h"

CLockUtil::CLockUtil(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);
}


CLockUtil::~CLockUtil(void)
{
}

BOOL CLockUtil::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	//if( log != NULL ){
	//	_OutputDebugString(L"��%s",log);
	//}
	DWORD dwRet;
	if( timeOut == INFINITE ){
		//30�b���b�N�ŗl�q�����Ă��疳���ōs��
		dwRet = WaitForSingleObject(this->lockEvent, 30*1000);
		if( dwRet == WAIT_ABANDONED || 
			dwRet == WAIT_FAILED ||
			dwRet == WAIT_TIMEOUT){
				if( log != NULL ){
					_OutputDebugString(L"��%s : Lock FALSE : (LastLock:%s)", log, lastLockLog.c_str());
				}else{
					_OutputDebugString(L"��Lock FALSE : (LastLock:%s)", lastLockLog.c_str());
				}
		}else{
			lastLockLog = log;
			return TRUE;
		}
	}
	dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED ||
		dwRet == WAIT_TIMEOUT){
			if( log != NULL ){
				_OutputDebugString(L"��%s : Lock FALSE : (LastLock:%s)", log, lastLockLog.c_str());
			}else{
				_OutputDebugString(L"��Lock FALSE : (LastLock:%s)", lastLockLog.c_str());
			}
		return FALSE;
	}
	lastLockLog = log;
	return TRUE;
}

void CLockUtil::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}
