// TstOpn.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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


// �B��̃A�v���P�[�V���� �I�u�W�F�N�g�ł��B

CWinApp theApp;

using namespace std;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// MFC �����������āA�G���[�̏ꍇ�͌��ʂ�������܂��B
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: �K�v�ɉ����ăG���[ �R�[�h��ύX���Ă��������B
			_tprintf(_T("�v���I�ȃG���[: MFC �̏��������ł��܂���ł����B\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: �A�v���P�[�V�����̓�����L�q����R�[�h�������ɑ}�����Ă��������B

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
		// TODO: �K�v�ɉ����ăG���[ �R�[�h��ύX���Ă��������B
		_tprintf(_T("�v���I�ȃG���[: GetModuleHandle �����s���܂���\n"));
		nRetCode = 1;
	}
	getchar();
	return 0;
	//return nRetCode;
}
