#include "StdAfx.h"
#include "ReNamePlugInUtil.h"

#include "Util.h"
#include "ErrDef.h"

CReNamePlugInUtil::CReNamePlugInUtil(void)
{
	module = NULL;

	this->lockEvent = _CreateEvent(FALSE, TRUE, _T("Global\\EpgTimerSrv_ReName_Event") );
}

CReNamePlugInUtil::~CReNamePlugInUtil(void)
{
	UnInitialize();

	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
}

BOOL CReNamePlugInUtil::Lock(LPCWSTR log, DWORD timeOut)
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

void CReNamePlugInUtil::UnLock(LPCWSTR log)
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
BOOL CReNamePlugInUtil::Initialize(
	LPCWSTR loadDllFilePath
	)
{
	if( module != NULL ){
		return FALSE;
	}
	if( Lock() == FALSE ) return FALSE;

	pfnGetPlugInNameRNP = NULL;
	pfnSettingRNP = NULL;
	pfnConvertRecNameRNP = NULL;
	pfnConvertRecName2RNP = NULL;


	BOOL ret = TRUE;

	module = ::LoadLibrary(loadDllFilePath);

	if( module == NULL ){
		OutputDebugString(L"dl�̃��[�h�Ɏ��s���܂���\r\n");
		UnLock();
		return FALSE;
	}

	pfnGetPlugInNameRNP = ( GetPlugInNameRNP ) ::GetProcAddress( module , "GetPlugInName");
	if( !pfnGetPlugInNameRNP ){
		OutputDebugString(L"GetPlugInName�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnSettingRNP = ( SettingRNP ) ::GetProcAddress( module , "Setting");
	if( !pfnSettingRNP ){
		OutputDebugString(L"Setting�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnConvertRecNameRNP = ( ConvertRecNameRNP ) ::GetProcAddress( module , "ConvertRecName");
	if( !pfnConvertRecNameRNP ){
		OutputDebugString(L"ConvertRecName�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnConvertRecName2RNP = ( ConvertRecName2RNP ) ::GetProcAddress( module , "ConvertRecName2");
	if( !pfnConvertRecName2RNP ){
		OutputDebugString(L"ConvertRecName�� GetProcAddress �Ɏ��s\r\n");
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
void CReNamePlugInUtil::UnInitialize()
{
	if( module != NULL ){
		::FreeLibrary( module );
	}
	module = NULL;

	pfnGetPlugInNameRNP = NULL;
	pfnSettingRNP = NULL;
	pfnConvertRecNameRNP = NULL;
	pfnConvertRecName2RNP = NULL;
}

//PlugIn�̖��O���擾����
//name��NULL���͕K�v�ȃT�C�Y��nameSize�ŕԂ�
//�ʏ�nameSize=256�ŌĂяo��
//�߂�l
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// name						[OUT]����
// nameSize					[IN/OUT]name�̃T�C�Y(WCHAR�P��)
BOOL CReNamePlugInUtil::GetPlugInName(
	WCHAR* name,
	DWORD* nameSize
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}

	return pfnGetPlugInNameRNP(name, nameSize);
}

//PlugIn�Őݒ肪�K�v�ȏꍇ�A�ݒ�p�̃_�C�A���O�Ȃǂ�\������
//�����F
// parentWnd				[IN]�e�E�C���h�E
void CReNamePlugInUtil::Setting(
	HWND parentWnd
	)
{
	if( module == NULL ){
		return ;
	}

	return pfnSettingRNP(parentWnd);
}

//���͂��ꂽ�\��������ɁA�^�掞�̃t�@�C�������쐬����i�g���q�܂ށj
//recName��NULL���͕K�v�ȃT�C�Y��recNamesize�ŕԂ�
//�ʏ�recNamesize=256�ŌĂяo��
//�߂�l
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// info						[IN]�\����
// recName					[OUT]����
// recNamesize				[IN/OUT]name�̃T�C�Y(WCHAR�P��)
BOOL CReNamePlugInUtil::ConvertRecName(
	PLUGIN_RESERVE_INFO* info,
	WCHAR* recName,
	DWORD* recNamesize
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}
	if( Lock() == FALSE ) return FALSE;

	BOOL ret = pfnConvertRecNameRNP(info, recName, recNamesize);

	UnLock();

	return ret;
}

//���͂��ꂽ�\��������ɁA�^�掞�̃t�@�C�������쐬����i�g���q�܂ށj
//recName��NULL���͕K�v�ȃT�C�Y��recNamesize�ŕԂ�
//�ʏ�recNamesize=256�ŌĂяo��
//�߂�l
// TRUE�i�����j�AFALSE�i���s�j
//�����F
// info						[IN]�\����
// epgInfo					[IN]�ԑg���iEPG�\��Ŕԑg��񂪑��݂��鎞�A���݂��Ȃ��ꍇ��NULL�j
// recName					[OUT]����
// recNamesize				[IN/OUT]name�̃T�C�Y(WCHAR�P��)
BOOL CReNamePlugInUtil::ConvertRecName2(
	PLUGIN_RESERVE_INFO* info,
	EPG_EVENT_INFO* epgInfo,
	WCHAR* recName,
	DWORD* recNamesize
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}
	if( Lock() == FALSE ) return FALSE;
	BOOL ret = FALSE;
	if( pfnConvertRecName2RNP != NULL ){
		ret = pfnConvertRecName2RNP(info, epgInfo, recName, recNamesize);
	}else{
		ret = pfnConvertRecNameRNP(info, recName, recNamesize);
	}

	UnLock();

	return ret;
}

