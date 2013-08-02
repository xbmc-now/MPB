
#include <windows.h>
#include <winsvc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "../Common/Util.h"
#include "my_global.h"
#include "mysql.h"


class CDbCtrl
{
public:
	CDbCtrl(void);
	~CDbCtrl(void);


	DWORD Connect(
		MYSQL *, 
		CString, 
		CString, 
		CString, 
		CString);

	DWORD Query(MYSQL *, CString);


	void Close(MYSQL *);

	DWORD StoreResult(MYSQL *, MYSQL_RES **);

	MYSQL_ROW FetchRow(MYSQL_RES **);

	void FreeResult(MYSQL_RES **);

protected:
	HANDLE lockEvent;
	HANDLE buffLockEvent;


protected:
	//PublicAPIîrëºêßå‰óp
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 15*1000);
	void UnLock(LPCWSTR log = NULL);


};

