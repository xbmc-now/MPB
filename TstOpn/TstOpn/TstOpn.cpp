// TstOpn.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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

/*
			const TCHAR user_name[] = L"root";       // DB�ɐڑ����郆�[�U��
			const TCHAR passwd[]    = L"MediaPortal";   // ���[�U�̃p�X���[�h
			const TCHAR db_name[]   = L"mptvdb"; // �ڑ���DB��
*/
			const char user_name[] = "root";       // DB�ɐڑ����郆�[�U��
			const char passwd[]    = "MediaPortal";   // ���[�U�̃p�X���[�h
			const char db_name[]   = "mptvdb"; // �ڑ���DB��


			// MySQL�\���̂́A1 �̃f�[�^�x�[�X�ڑ��ւ̃n���h����\���B
			// �قƂ�ǂ� MySQL �֐��Ŏg�p�����B 
			MYSQL mysql;

			// SELECT�̎��s���ʊi�[�p
			MYSQL_RES *results = NULL;

			// �i�[���R�[�h�^
			MYSQL_ROW record;

			_tprintf(L"���j�R�[�h�����\n");

			// MySQL�ڑ��n���h���̏�����
			mysql_init(&mysql);

			// �����R�[�h��ݒ肵�Ă���
			//mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "sjis");
			mysql_options(&mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");
			mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
			//mysql_set_character_set(&mysql, "utf8");
			//
			//mysql_set_character_set(&mysql, "utf8");
			//mysql_query(&mysql,"SET NAMES utf8");

			//mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
			
			// MySQL�ɐڑ�����
			mysql_real_connect(&mysql, NULL, user_name, passwd, db_name, 0, 0, 0);
			if (mysql_errno(&mysql) != 0) {
				_tprintf(L"�G���[����: %s\n", mysql_error(&mysql));
				return -1;
			}


			//mysql_options(&mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");
			// �������s
			//mysql_query(&mysql, "SELECT idCard, name FROM card;");
			mysql_query(&mysql, "SELECT idChannel, displayName FROM channel;");
			if (mysql_errno(&mysql) != 0) {
				_tprintf(L"�G���[����: %s\n", mysql_error(&mysql));
				return -2;
			}

			// �������ʎ擾
			results = mysql_store_result(&mysql);
			if (results == NULL || mysql_errno(&mysql) != 0) {
				_tprintf(L"�G���[����: %s\n", mysql_error(&mysql));
				return -3;
			}


			// ���R�[�h���Ȃ��Ȃ�܂Ō������ʂ�\������
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

			// �������ʊi�[�G���A���
			mysql_free_result(results);

			// MySQL�ւ̐ڑ���ؒf����
			mysql_close(&mysql);


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
