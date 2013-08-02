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

// データベース接続
//    戻り値：
//        エラーコード
//    引数：
//        [OUT]：ハンドル
//        [IN] ：ホスト
//        [IN] ：ユーザー
//        [IN] ：パスワード
//        [IN] ：データベース
DWORD CDbCtrl::Connect(
	MYSQL *mysql, 
	CString cshost, 
	CString csuser, 
	CString cspasswd, 
	CString csdb)
{
	// CString → const char(utf8)
	CT2CA host(cshost, CP_UTF8);
	CT2CA user(csuser, CP_UTF8);
	CT2CA passwd(cspasswd, CP_UTF8);
	CT2CA db(csdb, CP_UTF8);

	// MySQL接続ハンドラの初期化
	mysql_init(mysql);

	// 文字コードを設定しておく
	mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");

	// MySQLに接続する
	mysql_real_connect(mysql, host, user, passwd, db, 0, 0, 0);

	return mysql_errno(mysql);
}

// データベース切断
//    引数：
//        [OUT]：ハンドル
void CDbCtrl::Close(MYSQL *mysql)
{
	mysql_close(mysql);
}

// クエリ実行
//    戻り値：
//        エラーコード
//    引数：
//        [OUT]：ハンドル
//        [IN] ：SQL
DWORD CDbCtrl::Query(MYSQL *mysql, CString cssql)
{
	// CString → const char(utf8)
	CT2CA sql(cssql, CP_UTF8);
	mysql_query(mysql, sql);
	return mysql_errno(mysql);
}

// 結果取り出し
//    戻り値：
//        エラーコード
//    引数：
//        [OUT]：ハンドル
//        [OUT]：結果セット
DWORD CDbCtrl::StoreResult(MYSQL *mysql, MYSQL_RES **results)
{
	// 検索結果取得
	*results = mysql_store_result(mysql);
	return mysql_errno(mysql);
}

// 一行取り出し
//    戻り値：
//        MYSQL_ROW
//    引数：
//        [OUT]：結果セット
MYSQL_ROW CDbCtrl::FetchRow(MYSQL_RES **results)
{
	return mysql_fetch_row(*results);
}

// 結果解放
//    引数：
//        [OUT]：結果セット
void CDbCtrl::FreeResult(MYSQL_RES **results)
{
	// 検索結果格納エリア解放
	mysql_free_result(*results);
}
