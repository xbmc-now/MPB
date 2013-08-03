
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

	// �G���[�R�[�h�擾
	//    �߂�l�F
	//        �G���[�R�[�h
	//    �����F
	//        [OUT]�F�n���h��
	DWORD ErrNo(MYSQL *);

	// �f�[�^�x�[�X�ڑ�
	//    �߂�l�F
	//        �G���[�R�[�h
	//    �����F
	//        [OUT]�F�n���h��
	//        [IN] �F�z�X�g
	//        [IN] �F���[�U�[
	//        [IN] �F�p�X���[�h
	//        [IN] �F�f�[�^�x�[�X
	DWORD Connect(
		MYSQL *, 
		CString, 
		CString, 
		CString, 
		CString);

	// �f�[�^�x�[�X�ؒf
	//    �����F
	//        [OUT]�F�n���h��
	void Close(MYSQL *);

	// �N�G�����s
	//    �߂�l�F
	//        �G���[�R�[�h
	//    �����F
	//        [OUT]�F�n���h��
	//        [IN] �FSQL
	DWORD Query(MYSQL *, CString);

	// ���ʎ��o��
	//    �߂�l�F
	//        �G���[�R�[�h
	//    �����F
	//        [OUT]�F�n���h��
	//        [OUT]�F���ʃZ�b�g
	DWORD StoreResult(MYSQL *, MYSQL_RES **);

	// ��s���o��
	//    �߂�l�F
	//        MYSQL_ROW
	//    �����F
	//        [OUT]�F���ʃZ�b�g
	MYSQL_ROW FetchRow(MYSQL_RES **);

	// ���ʉ��
	//    �����F
	//        [OUT]�F���ʃZ�b�g
	void FreeResult(MYSQL_RES **);

	// ���b�N�e�[�u��
	//    �߂�l�F
	//        �G���[�R�[�h
	//    �����F
	//        [OUT]�F�n���h��
	//        [OUT]�F���b�N�e�[�u���n�b�V��(�e�[�u����,1:�ǂݍ��� 2:��������)
	DWORD LockTable(MYSQL *, map<CString, int> &);

	// �A�����b�N�e�[�u��
	//    �߂�l�F
	//        �G���[�R�[�h
	//    �����F
	//        [OUT]�F�n���h��
	DWORD UnlockTable(MYSQL *);

	// �g�����U�N�V�����J�n
	//    �߂�l�F
	//        �G���[�R�[�h
	//    �����F
	//        [OUT]�F�n���h��
	DWORD Begin(MYSQL *);

	// �R�~�b�g
	//    �߂�l�F
	//        �G���[�R�[�h
	//    �����F
	//        [OUT]�F�n���h��
	DWORD Commit(MYSQL *);

	// ���[���o�b�N
	//    �߂�l�F
	//        �G���[�R�[�h
	//    �����F
	//        [OUT]�F�n���h��
	DWORD Rollback(MYSQL *);

protected:
	HANDLE lockEvent;
	HANDLE buffLockEvent;


protected:
	//PublicAPI�r������p
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 15*1000);
	void UnLock(LPCWSTR log = NULL);


};

