#include "StdAfx.h"
#include "WritePlugInUtil.h"

#include "Util.h"
#include "ErrDef.h"

CWritePlugInUtil::CWritePlugInUtil(void)
{
	module = NULL;

	this->id = 0;
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL );
}

CWritePlugInUtil::~CWritePlugInUtil(void)
{
	UnInitialize();

	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
}

BOOL CWritePlugInUtil::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
	DWORD dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED){
		return FALSE;
	}
	return TRUE;
}

void CWritePlugInUtil::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

//DLL�̏�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// loadDllFilePath		[IN]���[�h����DLL�p�X
BOOL CWritePlugInUtil::Initialize(
	LPCWSTR loadDllFilePath
	)
{
	if( module != NULL ){
		return FALSE;
	}
	if( Lock() == FALSE ) return FALSE;

	pfnGetPlugInNameWP = NULL;
	pfnSettingWP = NULL;
	pfnCreateCtrlWP = NULL;
	pfnDeleteCtrlWP = NULL;
	pfnStartSaveWP = NULL;
	pfnStopSaveWP = NULL;
	pfnGetSaveFilePathWP = NULL;
	pfnAddTSBuffWP = NULL;


	BOOL ret = TRUE;

	module = ::LoadLibrary(loadDllFilePath);

	if( module == NULL ){
		OutputDebugString(L"dl�̃��[�h�Ɏ��s���܂���\r\n");
		UnLock();
		return FALSE;
	}

	pfnGetPlugInNameWP = ( GetPlugInNameWP ) ::GetProcAddress( module , "GetPlugInName");
	if( !pfnGetPlugInNameWP ){
		OutputDebugString(L"GetPlugInName�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnSettingWP = ( SettingWP ) ::GetProcAddress( module , "Setting");
	if( !pfnSettingWP ){
		OutputDebugString(L"Setting�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnCreateCtrlWP = ( CreateCtrlWP ) ::GetProcAddress( module , "CreateCtrl");
	if( !pfnCreateCtrlWP ){
		OutputDebugString(L"CreateCtrl�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnDeleteCtrlWP = ( DeleteCtrlWP ) ::GetProcAddress( module , "DeleteCtrl");
	if( !pfnDeleteCtrlWP ){
		OutputDebugString(L"DeleteCtrl�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnStartSaveWP = ( StartSaveWP ) ::GetProcAddress( module , "StartSave");
	if( !pfnStartSaveWP ){
		OutputDebugString(L"StartSave�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnStopSaveWP = ( StopSaveWP ) ::GetProcAddress( module , "StopSave");
	if( !pfnStopSaveWP ){
		OutputDebugString(L"StopSave�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetSaveFilePathWP = ( GetSaveFilePathWP ) ::GetProcAddress( module , "GetSaveFilePath");
	if( !pfnGetSaveFilePathWP ){
		OutputDebugString(L"GetSaveFilePath�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnAddTSBuffWP = ( AddTSBuffWP ) ::GetProcAddress( module , "AddTSBuff");
	if( !pfnAddTSBuffWP ){
		OutputDebugString(L"AddTSBuff�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}

	if( pfnCreateCtrlWP(&this->id) == FALSE ){
		this->id = 0;
		ret = FALSE;
		goto ERR_END;
	}

ERR_END:
	if( ret == FALSE ){
		::FreeLibrary( module );
		module=NULL;
	}
	UnLock();
	return ret;
}

//DLL�̊J��
//�߂�l�F
// �G���[�R�[�h
void CWritePlugInUtil::UnInitialize()
{
	if( module != NULL ){
		pfnDeleteCtrlWP(this->id);
		this->id = 0;
		::FreeLibrary( module );
	}
	module = NULL;
	
	pfnGetPlugInNameWP = NULL;
	pfnSettingWP = NULL;
	pfnCreateCtrlWP = NULL;
	pfnDeleteCtrlWP = NULL;
	pfnStartSaveWP = NULL;
	pfnStopSaveWP = NULL;
	pfnGetSaveFilePathWP = NULL;
	pfnAddTSBuffWP = NULL;

}

//PlugIn�̖��O���擾����
//name��NULL���͕K�v�ȃT�C�Y��nameSize�ŕԂ�
//�ʏ�nameSize=256�ŌĂяo��
//�߂�l
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// name						[OUT]����
// nameSize					[IN/OUT]name�̃T�C�Y(WCHAR�P��)
BOOL CWritePlugInUtil::GetPlugInName(
	WCHAR* name,
	DWORD* nameSize
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}

	return pfnGetPlugInNameWP(name, nameSize);
}

//PlugIn�Őݒ肪�K�v�ȏꍇ�A�ݒ�p�̃_�C�A���O�Ȃǂ�\������
//�����F
// parentWnd				[IN]�e�E�C���h�E
void CWritePlugInUtil::Setting(
	HWND parentWnd
	)
{
	if( module == NULL ){
		return ;
	}

	return pfnSettingWP(parentWnd);
}

//�t�@�C���ۑ����J�n����
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// fileName				[IN]�ۑ��t�@�C���t���p�X�i�K�v�ɉ����Ċg���q�ς�����ȂǍs���j
// overWriteFlag		[IN]����t�@�C�������ݎ��ɏ㏑�����邩�ǂ����iTRUE�F����AFALSE�F���Ȃ��j
// createSize			[IN]���͗\�z�e�ʁi188�o�C�gTS�ł̗e�ʁB�����^�掞�ȂǑ����Ԗ���̏ꍇ��0�B�����Ȃǂ̉\��������̂Ŗڈ����x�j
BOOL CWritePlugInUtil::StartSave(
	LPCWSTR fileName,
	BOOL overWriteFlag,
	ULONGLONG createSize
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = pfnStartSaveWP(this->id, fileName, overWriteFlag, createSize);

	UnLock();

	return ret;
}

//�t�@�C���ۑ����I������
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
BOOL CWritePlugInUtil::StopSave(
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = pfnStopSaveWP(this->id);

	UnLock();

	return ret;
}

//���ۂɕۑ����Ă���t�@�C���p�X���擾����i�Đ���o�b�`�����ɗ��p�����j
//filePath��NULL���͕K�v�ȃT�C�Y��filePathSize�ŕԂ�
//�ʏ�filePathSize=512�ŌĂяo��
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// filePath				[OUT]�ۑ��t�@�C���t���p�X
// filePathSize			[IN/OUT]filePath�̃T�C�Y(WCHAR�P��)
BOOL CWritePlugInUtil::GetSaveFilePath(
	WCHAR* filePath,
	DWORD* filePathSize
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = pfnGetSaveFilePathWP(this->id, filePath, filePathSize);

	UnLock();

	return ret;
}

//�ۑ��pTS�f�[�^�𑗂�
//�󂫗e�ʕs���Ȃǂŏ����o�����s�����ꍇ�AwriteSize�̒l������
//�ēx�ۑ���������Ƃ��̑��M�J�n�n�_�����߂�
//�߂�l�F
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// data					[IN]TS�f�[�^
// size					[IN]data�̃T�C�Y
// writeSize			[OUT]�ۑ��ɗ��p�����T�C�Y
BOOL CWritePlugInUtil::AddTSBuff(
	BYTE* data,
	DWORD size,
	DWORD* writeSize
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = pfnAddTSBuffWP(this->id, data, size, writeSize);

	UnLock();

	return ret;
}

