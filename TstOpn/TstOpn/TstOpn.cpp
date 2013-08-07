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
#include <regex>

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

		std::wstring text (L"ciacaaabccdacaaabcb");
		std::wregex re (L"a*(bc)");

		std::wsmatch m;

		if( std::regex_search(text, m, re) ) {
			std::wcout << m[1] << std::endl;
		} else {
			std::wcout << "no match." << std::endl;
		}


/*
		std::string tar("ciacaaabccdacaaabcb");
		std::regex pat("a*(bc)");
		std::smatch m;

		if( std::regex_search(tar.cbegin(), tar.cend(), m, pat) ) {
			std::cout << m[1] << std::endl;
			//for(auto it = m.begin(); it != m.end(); ++it) {
			//	std::cout << m << std::endl;
			//	//std::cout << it->str() << std::endl;
			//}
		} else {
			std::cout << "no match." << std::endl;
		}
*/
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
