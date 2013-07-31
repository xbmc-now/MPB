#pragma once

#include <windows.h>

#include "../Common/Util.h"
#include "my_global.h"
#include "mysql.h"


class CDbCtrl
{
public:
	CDbCtrl(void);
	~CDbCtrl(void);
	DWORD CDbCtrl::Connect(
		MYSQL *mysql, 
		const char *host, 
		const char *user, 
		const char *passwd, 
		const char *db);

protected:

	HANDLE lockEvent;

	HANDLE buffLockEvent;


protected:
	//PublicAPIîrëºêßå‰óp
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 15*1000);
	void UnLock(LPCWSTR log = NULL);


};

