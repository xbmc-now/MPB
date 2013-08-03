// TstOpn.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "TstOpn.h"
#include <iostream>
#include <winsock.h>
#include "my_global.h"
#include "mysql.h"
#include <string>
#include <map>

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



			std::map<CString,CString> data;


			//値を入れます
			data[L"inaba"]=L"いなば";

			// 要素を追加する
			data.insert( map<CString,CString>::value_type( L"minoru", L"みのる" ) );

			// 要素を出力する
			map<CString,CString>::iterator it = data.begin();
			while( it != data.end() )
			{
				_tprintf((*it).first)s;
				_tprintf(data[(*it).first]);
				//cout << (*it).first << ":" << (*it).second << endl;
				++it;
			}

			// 要素数を出力する
			cout << "要素数：" << (unsigned int)data.size() << endl;

			// 要素の全削除
			data.clear();

			// 全削除されているか確認
			if( data.empty() )
			{
				cout << "空です" << endl;
			}

/*
#include <map>
#include <string>
using namespace std;
 
void SetMyName(map<int, string> &mapBuff)
{
	mapBuff[5] = "私の名前"
}
 
void main()
{
	map<int, string> mapName;
 
	SetMyName(mapName);
	printf("%s\n", mapName[5].c_str());
}
*/

			std::map<int,CString> data2;

			data2[2]="２です";//値を入れます
			data2[3]="３です";



			_tprintf(L"%s",data2[3]);

/*
			// 同じ
			const TCHAR tst_ctchar1[] = L"日本語";
			const TCHAR *tst_ctchar2 = L"二本土";
			LPCTSTR tst_ctchar3 = _T("こここ");
			
			_tprintf(L"%s",tst_ctchar1);
			_tprintf(L"%s",tst_ctchar2);
			_tprintf(L"%s",tst_ctchar3);
*/


/*
			// 同じ
			TCHAR tst_tchar1[] = L"日本語";
			TCHAR *tst_tchar2 = L"二本土";
			LPTSTR tst_tchar3 = _T("こここ");

			_tprintf(L"%s",tst_tchar1);
			_tprintf(L"%s",tst_tchar2);
			_tprintf(L"%s",tst_tchar3);
*/

/*
			// 同じ
			const char tst_cchar1[] = "んの";
			const char *tst_cchar2 = "ooooo";
			LPCSTR tst_cchar3 = "こここ";

			printf("%s",tst_cchar1);
			printf("%s",tst_cchar2);
			printf("%s",tst_cchar3);
*/

/*
			// 同じ
			char tst_char1[] = "んの";
			char *tst_char2 = "ooooo";
			LPSTR tst_char3 = "こここ";

			printf("%s",tst_char1);
			printf("%s",tst_char2);
			printf("%s",tst_char3);
*/

			// 便利CString(TCHAR拡張)
			//CString tst_cstr = L"日本語";

			// 変換マクロATL
			// TCHAR→char
			//CT2A tst_mt2a(L"サンプルT");
			//printf("%s",tst_mt2a);

			// char→TCHAR
			//CA2T tst_ma2t("サンプルA");
			//_tprintf(L"%s",tst_ma2t);

			// TCHAR→char(utf8)
			//CT2A tst_mt2autf8(_T("サンプルUTF8"),CP_UTF8);
			//printf("%s",tst_mt2autf8);

			// TCHAR→const char(utf8)
			//CT2CA tst_mt2cautf8(_T("サンプルUTF8"),CP_UTF8);
			//printf("%s",tst_mt2cautf8);


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
