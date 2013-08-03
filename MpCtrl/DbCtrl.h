
#include <windows.h>
#include "../Common/Util.h"
#include "my_global.h"
#include "mysql.h"
#include <map>
using namespace std;

class CDbCtrl
{
public:
	CDbCtrl(void);
	~CDbCtrl(void);

	// データベース接続
	//    戻り値：
	//        エラーコード
	//    引数：
	//        [OUT]：ハンドル
	//        [IN] ：ホスト
	//        [IN] ：ユーザー
	//        [IN] ：パスワード
	//        [IN] ：データベース
	DWORD Connect(
		MYSQL *, 
		CString, 
		CString, 
		CString, 
		CString);

	// データベース切断
	//    引数：
	//        [OUT]：ハンドル
	void Close(MYSQL *);

	// クエリ実行
	//    戻り値：
	//        エラーコード
	//    引数：
	//        [OUT]：ハンドル
	//        [IN] ：SQL
	DWORD Query(MYSQL *, CString);

	// 結果取り出し
	//    戻り値：
	//        エラーコード
	//    引数：
	//        [OUT]：ハンドル
	//        [OUT]：結果セット
	DWORD StoreResult(MYSQL *, MYSQL_RES **);

	// 一行取り出し
	//    戻り値：
	//        MYSQL_ROW
	//    引数：
	//        [OUT]：結果セット
	MYSQL_ROW FetchRow(MYSQL_RES **);

	// 結果解放
	//    引数：
	//        [OUT]：結果セット
	void FreeResult(MYSQL_RES **);

	void SetMyName(map<CString, CString> &);

protected:
	HANDLE lockEvent;
	HANDLE buffLockEvent;


protected:
	//PublicAPI排他制御用
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 15*1000);
	void UnLock(LPCWSTR log = NULL);


};

