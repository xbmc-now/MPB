#include "StdAfx.h"
#include "EpgDataCap3Util.h"

#include "ErrDef.h"

CEpgDataCap3Util::CEpgDataCap3Util(void)
{
	module = NULL;
	id = 0;
}

CEpgDataCap3Util::~CEpgDataCap3Util(void)
{
	UnLoadDll();
}

BOOL CEpgDataCap3Util::LoadDll(void)
{
	if( module != NULL ){
		return FALSE;
	}

	pfnInitializeEP3 = NULL;
	pfnUnInitializeEP3 = NULL;
	pfnAddTSPacketEP3 = NULL;
	pfnGetTSIDEP3 = NULL;
	pfnGetEpgInfoListEP3 = NULL;
	pfnClearSectionStatusEP3 = NULL;
	pfnGetSectionStatusEP3 = NULL;
	pfnGetServiceListActualEP3 = NULL;
	pfnGetServiceListEpgDBEP3 = NULL;
	pfnGetEpgInfoEP3 = NULL;
	pfnSearchEpgInfoEP3 = NULL;
	pfnGetTimeDelayEP3 = NULL;


	BOOL ret = TRUE;

	module = ::LoadLibrary(L"EpgDataCap3.dll");

	if( module == NULL ){
		OutputDebugString(L"EpgDataCap3.dll�̃��[�h�Ɏ��s���܂���\r\n");
		return FALSE;
	}

	pfnInitializeEP3 = ( InitializeEP3 ) ::GetProcAddress( module , "InitializeEP");
	if( !pfnInitializeEP3 ){
		OutputDebugString(L"InitializeEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnUnInitializeEP3 = ( UnInitializeEP3 ) ::GetProcAddress( module , "UnInitializeEP");
	if( !pfnUnInitializeEP3 ){
		OutputDebugString(L"UnInitializeEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnAddTSPacketEP3 = ( AddTSPacketEP3 ) ::GetProcAddress( module , "AddTSPacketEP");
	if( !pfnAddTSPacketEP3 ){
		OutputDebugString(L"AddTSPacketEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetTSIDEP3 = ( GetTSIDEP3 ) ::GetProcAddress( module , "GetTSIDEP");
	if( !pfnGetTSIDEP3 ){
		OutputDebugString(L"GetTSIDEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetEpgInfoListEP3 = ( GetEpgInfoListEP3 ) ::GetProcAddress( module , "GetEpgInfoListEP");
	if( !pfnGetEpgInfoListEP3 ){
		OutputDebugString(L"GetEpgInfoListEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnClearSectionStatusEP3 = ( ClearSectionStatusEP3 ) ::GetProcAddress( module , "ClearSectionStatusEP");
	if( !pfnClearSectionStatusEP3 ){
		OutputDebugString(L"ClearSectionStatusEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetSectionStatusEP3 = ( GetSectionStatusEP3 ) ::GetProcAddress( module , "GetSectionStatusEP");
	if( !pfnGetSectionStatusEP3 ){
		OutputDebugString(L"GetSectionStatusEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetServiceListActualEP3 = ( GetServiceListActualEP3 ) ::GetProcAddress( module , "GetServiceListActualEP");
	if( !pfnGetServiceListActualEP3 ){
		OutputDebugString(L"GetServiceListActualEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetServiceListEpgDBEP3 = ( GetServiceListEpgDBEP3 ) ::GetProcAddress( module , "GetServiceListEpgDBEP");
	if( !pfnGetServiceListEpgDBEP3 ){
		OutputDebugString(L"GetServiceListEpgDBEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetEpgInfoEP3 = ( GetEpgInfoEP3 ) ::GetProcAddress( module , "GetEpgInfoEP");
	if( !pfnGetEpgInfoEP3 ){
		OutputDebugString(L"GetEpgInfoEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnSearchEpgInfoEP3 = ( SearchEpgInfoEP3 ) ::GetProcAddress( module , "SearchEpgInfoEP");
	if( !pfnSearchEpgInfoEP3 ){
		OutputDebugString(L"SearchEpgInfoEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetTimeDelayEP3 = ( GetTimeDelayEP3 ) ::GetProcAddress( module , "GetTimeDelayEP");
	if( !pfnGetTimeDelayEP3 ){
		OutputDebugString(L"GetTimeDelayEP�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}


ERR_END:
	if( ret == FALSE ){
		::FreeLibrary( module );
		module=NULL;
	}
	return ret;
}

BOOL CEpgDataCap3Util::UnLoadDll(void)
{
	if( module != NULL ){
		if( id != 0 ){
			pfnUnInitializeEP3(id);
		}
		::FreeLibrary( module );
		id = 0;
	}
	module = NULL;

	pfnInitializeEP3 = NULL;
	pfnUnInitializeEP3 = NULL;
	pfnAddTSPacketEP3 = NULL;
	pfnGetTSIDEP3 = NULL;
	pfnGetEpgInfoListEP3 = NULL;
	pfnClearSectionStatusEP3 = NULL;
	pfnGetSectionStatusEP3 = NULL;
	pfnGetServiceListActualEP3 = NULL;
	pfnGetServiceListEpgDBEP3 = NULL;
	pfnGetEpgInfoEP3 = NULL;
	pfnSearchEpgInfoEP3 = NULL;
	pfnGetTimeDelayEP3 = NULL;

	return TRUE;
}

//DLL�̏�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// asyncMode		[IN]TRUE:�񓯊����[�h�AFALSE:�������[�h
DWORD CEpgDataCap3Util::Initialize(
	BOOL asyncFlag
	)
{
	if( LoadDll() == FALSE ){
		return ERR_INIT;
	}
	DWORD err = pfnInitializeEP3(asyncFlag, &id);
	if( err != NO_ERR ){
		id = 0;
	}
	return err;
}

//DLL�̊J��
//�߂�l�F
// �G���[�R�[�h
DWORD CEpgDataCap3Util::UnInitialize(
	)
{
	if( module == NULL || id == 0 ){
		return ERR_NOT_INIT;
	}
	DWORD err = pfnUnInitializeEP3(id);
	UnLoadDll();
	return err;
}

//��͑Ώۂ�TS�p�P�b�g�P��ǂݍ��܂���
//�߂�l�F
// �G���[�R�[�h
//�����F
// data		[IN]TS�p�P�b�g�P��
// size		[IN]data�̃T�C�Y�i188�A192������ɂȂ�͂��j
DWORD CEpgDataCap3Util::AddTSPacket(
	BYTE* data,
	DWORD size
	)
{
	if( module == NULL || id == 0 ){
		return ERR_NOT_INIT;
	}
	return pfnAddTSPacketEP3(id, data, size);
}

//��̓f�[�^�̌��݂̃X�g���[���h�c���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[OUT]���݂�originalNetworkID
// transportStreamID		[OUT]���݂�transportStreamID
DWORD CEpgDataCap3Util::GetTSID(
	WORD* originalNetworkID,
	WORD* transportStreamID
	)
{
	if( module == NULL || id == 0 ){
		return ERR_NOT_INIT;
	}
	return pfnGetTSIDEP3(id, originalNetworkID, transportStreamID);
}

//���X�g���[���̃T�[�r�X�ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// serviceListSize			[OUT]serviceList�̌�
// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap3Util::GetServiceListActual(
	DWORD* serviceListSize,
	SERVICE_INFO** serviceList
	)
{
	if( module == NULL || id == 0 ){
		return ERR_NOT_INIT;
	}
	return pfnGetServiceListActualEP3(id, serviceListSize, serviceList);
}

//�~�ς��ꂽEPG���̂���T�[�r�X�ꗗ���擾����
//SERVICE_EXT_INFO�̏��͂Ȃ�
//�߂�l�F
// �G���[�R�[�h
//�����F
// serviceListSize			[OUT]serviceList�̌�
// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap3Util::GetServiceListEpgDB(
	DWORD* serviceListSize,
	SERVICE_INFO** serviceList
	)
{
	if( module == NULL || id == 0 ){
		return ERR_NOT_INIT;
	}
	return pfnGetServiceListEpgDBEP3(id, serviceListSize, serviceList);
}

//�w��T�[�r�X�̑SEPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// epgInfoListSize			[OUT]epgInfoList�̌�
// epgInfoList				[OUT]EPG���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap3Util::GetEpgInfoList(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	DWORD* epgInfoListSize,
	EPG_EVENT_INFO** epgInfoList
	)
{
	if( module == NULL || id == 0 ){
		return ERR_NOT_INIT;
	}
	return pfnGetEpgInfoListEP3(id, originalNetworkID, transportStreamID, serviceID, epgInfoListSize, epgInfoList);
}

//EPG�f�[�^�̒~�Ϗ�Ԃ����Z�b�g����
void CEpgDataCap3Util::ClearSectionStatus()
{
	if( module == NULL || id == 0 ){
		return ;
	}
	pfnClearSectionStatusEP3(id);
}

//EPG�f�[�^�̒~�Ϗ�Ԃ��擾����
//�߂�l�F
// �X�e�[�^�X
//�����F
// l_eitFlag		[IN]L-EIT�̃X�e�[�^�X���擾
EPG_SECTION_STATUS CEpgDataCap3Util::GetSectionStatus(
	BOOL l_eitFlag
	)
{
	if( module == NULL || id == 0 ){
		return EpgNoData;
	}
	return pfnGetSectionStatusEP3(id, l_eitFlag);
}

//�w��T�[�r�X�̌���or����EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// nextFlag					[IN]TRUE�i���̔ԑg�j�AFALSE�i���݂̔ԑg�j
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap3Util::GetEpgInfo(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	BOOL nextFlag,
	EPG_EVENT_INFO** epgInfo
	)
{
	if( module == NULL || id == 0 ){
		return ERR_NOT_INIT;
	}
	return pfnGetEpgInfoEP3(id, originalNetworkID, transportStreamID, serviceID, nextFlag, epgInfo);
}

//�w��C�x���g��EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// EventID					[IN]�擾�Ώۂ�EventID
// pfOnlyFlag				[IN]p/f����̂݌������邩�ǂ���
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap3Util::SearchEpgInfo(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	WORD eventID,
	BYTE pfOnlyFlag,
	EPG_EVENT_INFO** epgInfo
	)
{
	if( module == NULL || id == 0 ){
		return ERR_NOT_INIT;
	}
	return pfnSearchEpgInfoEP3(id, originalNetworkID, transportStreamID, serviceID, eventID, pfOnlyFlag, epgInfo);
}

//PC���v�����Ƃ����X�g���[�����ԂƂ̍����擾����
//�߂�l�F
// ���̕b��
int CEpgDataCap3Util::GetTimeDelay(
	)
{
	if( module == NULL || id == 0 ){
		return 0;
	}
	return pfnGetTimeDelayEP3(id);
}
