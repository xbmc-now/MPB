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

// エラーコード取得
//    戻り値：
//        エラーコード
//    引数：
//        [OUT]：ハンドル
DWORD CDbCtrl::ErrNo(MYSQL *mysql)
{
	return mysql_errno(mysql);
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

	return this->ErrNo(mysql);
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
	return this->ErrNo(mysql);
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
	return this->ErrNo(mysql);
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

// 行数
//    戻り値：
//        行数
//    引数：
//        [OUT]：結果セット
DWORD CDbCtrl::NumRows(MYSQL_RES **results)
{
	return mysql_num_rows(*results);
}

// 結果解放
//    引数：
//        [OUT]：結果セット
void CDbCtrl::FreeResult(MYSQL_RES **results)
{
	// 検索結果格納エリア解放
	mysql_free_result(*results);
}

// ロックテーブル
//    戻り値：
//        エラーコード
//    引数：
//        [OUT]：ハンドル
//        [OUT]：ロックテーブルハッシュ(テーブル名,1:読み込み 2:書き込み)
DWORD CDbCtrl::LockTable(MYSQL *mysql, map<CString, int> &tblHash)
{
	std::map<int,CString> modeArr;
	modeArr[1] = _T("READ");
	modeArr[2] = _T("WRITE");

	CString sql = _T("LOCK TABLES ");
	map<CString,int>::iterator it = tblHash.begin();
	while( it != tblHash.end() )
	{
		sql += (*it).first + _T(" ") + modeArr[tblHash[(*it).first]];
		++it;
		if( it != tblHash.end() ) sql += _T(", ");
	}
	this->Query(mysql, sql);
	return this->ErrNo(mysql);
}

// アンロックテーブル
//    戻り値：
//        エラーコード
//    引数：
//        [OUT]：ハンドル
DWORD CDbCtrl::UnlockTable(MYSQL *mysql)
{
	CString sql = _T("UNLOCK TABLES");
	this->Query(mysql, sql);
	return this->ErrNo(mysql);
}

// トランザクション開始
//    戻り値：
//        エラーコード
//    引数：
//        [OUT]：ハンドル
DWORD CDbCtrl::Begin(MYSQL *mysql)
{
	CString sql = _T("BEGIN");
	this->Query(mysql, sql);
	return this->ErrNo(mysql);
}

// コミット
//    戻り値：
//        エラーコード
//    引数：
//        [OUT]：ハンドル
DWORD CDbCtrl::Commit(MYSQL *mysql)
{
	CString sql = _T("COMMIT");
	this->Query(mysql, sql);
	return this->ErrNo(mysql);
}

// ロールバック
//    戻り値：
//        エラーコード
//    引数：
//        [OUT]：ハンドル
DWORD CDbCtrl::Rollback(MYSQL *mysql)
{
	CString sql = _T("ROLLBACK");
	this->Query(mysql, sql);
	return this->ErrNo(mysql);
}

DWORD CDbCtrl::EscapeString(MYSQL *mysql, CString &csesc, CString cssrc, DWORD size)
{
	// CString → const char(utf8)
	CT2CA src(cssrc, CP_UTF8);
	
	char *esc;
	mysql_real_escape_string(mysql, esc, src, size);
	CA2CT tmpesc(esc);
	csesc = tmpesc;
	return this->ErrNo(mysql);
}

