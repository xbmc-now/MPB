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
				_OutputDebugString(L"��CDbCtrl::Lock FALSE : %s", log);
			}else{
				OutputDebugString(L"��CDbCtrl::Lock FALSE");
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

// �G���[�R�[�h�擾
//    �߂�l�F
//        �G���[�R�[�h
//    �����F
//        [OUT]�F�n���h��
DWORD CDbCtrl::ErrNo(MYSQL *mysql)
{
	return mysql_errno(mysql);
}

// �f�[�^�x�[�X�ڑ�
//    �߂�l�F
//        �G���[�R�[�h
//    �����F
//        [OUT]�F�n���h��
//        [IN] �F�z�X�g
//        [IN] �F���[�U�[
//        [IN] �F�p�X���[�h
//        [IN] �F�f�[�^�x�[�X
DWORD CDbCtrl::Connect(
	MYSQL *mysql, 
	CString cshost, 
	CString csuser, 
	CString cspasswd, 
	CString csdb)
{
	// CString �� const char(utf8)
	CT2CA host(cshost, CP_UTF8);
	CT2CA user(csuser, CP_UTF8);
	CT2CA passwd(cspasswd, CP_UTF8);
	CT2CA db(csdb, CP_UTF8);

	// MySQL�ڑ��n���h���̏�����
	mysql_init(mysql);

	// �����R�[�h��ݒ肵�Ă���
	mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");

	// MySQL�ɐڑ�����
	mysql_real_connect(mysql, host, user, passwd, db, 0, 0, 0);

	return this->ErrNo(mysql);
}

// �f�[�^�x�[�X�ؒf
//    �����F
//        [OUT]�F�n���h��
void CDbCtrl::Close(MYSQL *mysql)
{
	mysql_close(mysql);
}

// �N�G�����s
//    �߂�l�F
//        �G���[�R�[�h
//    �����F
//        [OUT]�F�n���h��
//        [IN] �FSQL
DWORD CDbCtrl::Query(MYSQL *mysql, CString cssql)
{
	// CString �� const char(utf8)
	CT2CA sql(cssql, CP_UTF8);
	mysql_query(mysql, sql);
	return this->ErrNo(mysql);
}

// ���ʎ��o��
//    �߂�l�F
//        �G���[�R�[�h
//    �����F
//        [OUT]�F�n���h��
//        [OUT]�F���ʃZ�b�g
DWORD CDbCtrl::StoreResult(MYSQL *mysql, MYSQL_RES **results)
{
	// �������ʎ擾
	*results = mysql_store_result(mysql);
	return this->ErrNo(mysql);
}

// ��s���o��
//    �߂�l�F
//        MYSQL_ROW
//    �����F
//        [OUT]�F���ʃZ�b�g
MYSQL_ROW CDbCtrl::FetchRow(MYSQL_RES **results)
{
	return mysql_fetch_row(*results);
}

// �s��
//    �߂�l�F
//        �s��
//    �����F
//        [OUT]�F���ʃZ�b�g
DWORD CDbCtrl::NumRows(MYSQL_RES **results)
{
	return mysql_num_rows(*results);
}

// ���ʉ��
//    �����F
//        [OUT]�F���ʃZ�b�g
void CDbCtrl::FreeResult(MYSQL_RES **results)
{
	// �������ʊi�[�G���A���
	mysql_free_result(*results);
}

// ���b�N�e�[�u��
//    �߂�l�F
//        �G���[�R�[�h
//    �����F
//        [OUT]�F�n���h��
//        [OUT]�F���b�N�e�[�u���n�b�V��(�e�[�u����,1:�ǂݍ��� 2:��������)
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

// �A�����b�N�e�[�u��
//    �߂�l�F
//        �G���[�R�[�h
//    �����F
//        [OUT]�F�n���h��
DWORD CDbCtrl::UnlockTable(MYSQL *mysql)
{
	CString sql = _T("UNLOCK TABLES");
	this->Query(mysql, sql);
	return this->ErrNo(mysql);
}

// �g�����U�N�V�����J�n
//    �߂�l�F
//        �G���[�R�[�h
//    �����F
//        [OUT]�F�n���h��
DWORD CDbCtrl::Begin(MYSQL *mysql)
{
	CString sql = _T("BEGIN");
	this->Query(mysql, sql);
	return this->ErrNo(mysql);
}

// �R�~�b�g
//    �߂�l�F
//        �G���[�R�[�h
//    �����F
//        [OUT]�F�n���h��
DWORD CDbCtrl::Commit(MYSQL *mysql)
{
	CString sql = _T("COMMIT");
	this->Query(mysql, sql);
	return this->ErrNo(mysql);
}

// ���[���o�b�N
//    �߂�l�F
//        �G���[�R�[�h
//    �����F
//        [OUT]�F�n���h��
DWORD CDbCtrl::Rollback(MYSQL *mysql)
{
	CString sql = _T("ROLLBACK");
	this->Query(mysql, sql);
	return this->ErrNo(mysql);
}

DWORD CDbCtrl::EscapeString(MYSQL *mysql, CString &csesc, CString cssrc, DWORD size)
{
	// CString �� const char(utf8)
	CT2CA src(cssrc, CP_UTF8);
	
	char *esc;
	mysql_real_escape_string(mysql, esc, src, size);
	CA2CT tmpesc(esc);
	csesc = tmpesc;
	return this->ErrNo(mysql);
}

