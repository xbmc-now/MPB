// TstOpn.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "TstOpn.h"
#include <iostream>
#include <winsock.h>
#include "my_global.h"
#include "mysql.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一のアプリケーション オブジェクトです。

CWinApp theApp;

using namespace std;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// MFC を初期化して、エラーの場合は結果を印刷します。
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 必要に応じてエラー コードを変更してください。
			_tprintf(_T("致命的なエラー: MFC の初期化ができませんでした。\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: アプリケーションの動作を記述するコードをここに挿入してください。

			_tsetlocale(LC_ALL, _T("Japanese_Japan.932"));

//------------------------------------------------------------------------------

/*
			const TCHAR user_name[] = L"root";       // DBに接続するユーザ名
			const TCHAR passwd[]    = L"MediaPortal";   // ユーザのパスワード
			const TCHAR db_name[]   = L"mptvdb"; // 接続先DB名
*/
			const char user_name[] = "root";       // DBに接続するユーザ名
			const char passwd[]    = "MediaPortal";   // ユーザのパスワード
			const char db_name[]   = "mptvdb"; // 接続先DB名


			// MySQL構造体は、1 つのデータベース接続へのハンドルを表す。
			// ほとんどの MySQL 関数で使用される。 
			MYSQL mysql;

			// SELECTの実行結果格納用
			MYSQL_RES *results = NULL;

			// 格納レコード型
			MYSQL_ROW record;

			_tprintf(L"ユニコードだよね\n");

			// MySQL接続ハンドラの初期化
			mysql_init(&mysql);

			// 文字コードを設定しておく
			//mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "sjis");
			mysql_options(&mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");
			mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
			//mysql_set_character_set(&mysql, "utf8");
			//
			//mysql_set_character_set(&mysql, "utf8");
			//mysql_query(&mysql,"SET NAMES utf8");

			//mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
			
			// MySQLに接続する
			mysql_real_connect(&mysql, NULL, user_name, passwd, db_name, 0, 0, 0);
			if (mysql_errno(&mysql) != 0) {
				_tprintf(L"エラー発生: %s\n", mysql_error(&mysql));
				return -1;
			}


			//mysql_options(&mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");
			// 検索実行
			//mysql_query(&mysql, "SELECT idCard, name FROM card;");
			mysql_query(&mysql, "SELECT idChannel, displayName FROM channel;");
			if (mysql_errno(&mysql) != 0) {
				_tprintf(L"エラー発生: %s\n", mysql_error(&mysql));
				return -2;
			}

			// 検索結果取得
			results = mysql_store_result(&mysql);
			if (results == NULL || mysql_errno(&mysql) != 0) {
				_tprintf(L"エラー発生: %s\n", mysql_error(&mysql));
				return -3;
			}


			// レコードがなくなるまで検索結果を表示する
			while (record = mysql_fetch_row(results)) {

				//printf("%s %s\n", record[0], record[1]);
				_tprintf(L"%s %s\n", CA2W(record[0], CP_UTF8), CA2W(record[1], CP_UTF8));
			}

/*
			unsigned int num_fields;
			unsigned int i;

			num_fields = mysql_num_fields(results);
			while ((record = mysql_fetch_row(results)))
			{
				unsigned long *lengths;
				lengths = mysql_fetch_lengths(results);
				for(i = 0; i < num_fields; i++)
				{
					//printf("[%.*s] ", (int) lengths[i], record[i] ? record[i] : "NULL");
					_tprintf(L"[%.*s] ", (int) lengths[i], record[i] ? record[i] : "NULL");
				}
				printf("\n");
			}
*/

			// 検索結果格納エリア解放
			mysql_free_result(results);

			// MySQLへの接続を切断する
			mysql_close(&mysql);


//------------------------------------------------------------------------------
			_tprintf(_T("Hello World!!\n"));
		}
	}
	else
	{
		// TODO: 必要に応じてエラー コードを変更してください。
		_tprintf(_T("致命的なエラー: GetModuleHandle が失敗しました\n"));
		nRetCode = 1;
	}
	getchar();
	return 0;
	//return nRetCode;
}
