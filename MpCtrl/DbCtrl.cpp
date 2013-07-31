#include "stdafx.h"
#include "DbCtrl.h"
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
				_OutputDebugString(L"◆CDbCtrl::Lock FALSE : %s", log);
			}else{
				OutputDebugString(L"◆CDbCtrl::Lock FALSE");
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

/*
DWORD CDbCtrl::Connect(
	MYSQL *mysql, 
	const char *host, 
	const char *user, 
	const char *passwd, 
	const char *db)
{
	
	// MySQL接続ハンドラの初期化
	mysql_init(&mysql);

	// 文字コードを設定しておく
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(&mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");

	// MySQLに接続する
	mysql_real_connect(&mysql, host, user, passwd, db, 0, NULL, 0);

	return mysql_errno(&mysql);
}
*/
