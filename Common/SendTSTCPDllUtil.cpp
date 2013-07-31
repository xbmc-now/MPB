#include "stdafx.h"
#include "SendTSTCPDllUtil.h"

CSendTSTCPDllUtil::CSendTSTCPDllUtil(void)
{
	m_hModule = NULL;
	m_iID = -1;
}

CSendTSTCPDllUtil::~CSendTSTCPDllUtil(void)
{
	UnLoadDll();
}

BOOL CSendTSTCPDllUtil::LoadDll(void)
{
	if( m_hModule != NULL ){
		return FALSE;
	}

	pfnInitializeDLL = NULL;
	pfnUnInitializeDLL = NULL;
	pfnAddSendAddrDLL = NULL;
	pfnClearSendAddrDLL = NULL;
	pfnStartSendDLL = NULL;
	pfnStopSendDLL = NULL;
	pfnAddSendDataDLL = NULL;
	pfnClearSendBuffDLL = NULL;

	BOOL bRet = TRUE;

	m_hModule = ::LoadLibrary(L"SendTSTCP.dll");

	if( m_hModule == NULL ){
//		::MessageBox( NULL, L"EpgDataCap.dll �̃��[�h�Ɏ��s���܂���", NULL, MB_OK);
		return FALSE;
	}

	pfnInitializeDLL = ( InitializeDLL ) ::GetProcAddress( m_hModule , "InitializeDLL");
	if( !pfnInitializeDLL ){
		bRet = FALSE;
		goto ERR_END;
	}
	pfnUnInitializeDLL = ( UnInitializeDLL ) ::GetProcAddress( m_hModule , "UnInitializeDLL");
	if( !pfnUnInitializeDLL ){
		bRet = FALSE;
		goto ERR_END;
	}
	pfnAddSendAddrDLL = ( AddSendAddrDLL ) ::GetProcAddress( m_hModule , "AddSendAddrDLL");
	if( !pfnAddSendAddrDLL ){
		bRet = FALSE;
		goto ERR_END;
	}
	pfnClearSendAddrDLL = ( ClearSendAddrDLL ) ::GetProcAddress( m_hModule , "ClearSendAddrDLL");
	if( !pfnClearSendAddrDLL ){
		bRet = FALSE;
		goto ERR_END;
	}
	pfnStartSendDLL = ( StartSendDLL ) ::GetProcAddress( m_hModule , "StartSendDLL");
	if( !pfnStartSendDLL ){
		bRet = FALSE;
		goto ERR_END;
	}
	pfnStopSendDLL = ( StopSendDLL ) ::GetProcAddress( m_hModule , "StopSendDLL");
	if( !pfnStopSendDLL ){
		bRet = FALSE;
		goto ERR_END;
	}
	pfnAddSendDataDLL = ( AddSendDataDLL ) ::GetProcAddress( m_hModule , "AddSendDataDLL");
	if( !pfnAddSendDataDLL ){
		bRet = FALSE;
		goto ERR_END;
	}
	pfnClearSendBuffDLL = ( ClearSendBuffDLL ) ::GetProcAddress( m_hModule , "ClearSendBuffDLL");
	if( !pfnClearSendBuffDLL ){
		bRet = FALSE;
		goto ERR_END;
	}

ERR_END:
	if( bRet == FALSE ){
		::FreeLibrary( m_hModule );
		m_hModule=NULL;
		::MessageBox( NULL, L"GetProcAddress �Ɏ��s���܂���", NULL, MB_OK);
	}
	return bRet;
}

BOOL CSendTSTCPDllUtil::UnLoadDll(void)
{
	if( m_hModule != NULL ){
		if( m_iID != -1 ){
			pfnUnInitializeDLL(m_iID);
		}
		::FreeLibrary( m_hModule );
		m_iID = -1;
	}
	m_hModule=NULL;
	return TRUE;
}

DWORD CSendTSTCPDllUtil::Initialize()
{
	if( LoadDll() == FALSE ){
		return ERR_INIT;
	}
	m_iID = pfnInitializeDLL();
	if( m_iID == -1 ){
		return ERR_INIT;
	}
	return NO_ERR;
}

void CSendTSTCPDllUtil::UnInitialize()
{
	if( m_hModule == NULL || m_iID == -1 ){
		return ;
	}
	DWORD dwRet = pfnUnInitializeDLL(m_iID);
	m_iID = -1;
	UnLoadDll();
	return ;
}

//���M���ǉ�
//�߂�l�F�G���[�R�[�h
DWORD CSendTSTCPDllUtil::AddSendAddr(
	LPCWSTR lpcwszIP,
	DWORD dwPort
	)
{
	if( m_hModule == NULL || m_iID == -1 ){
		return ERR_INIT;
	}
	return pfnAddSendAddrDLL(m_iID, lpcwszIP, dwPort);
}

//���M��N���A
//�߂�l�F�G���[�R�[�h
DWORD CSendTSTCPDllUtil::ClearSendAddr(
	)
{
	if( m_hModule == NULL || m_iID == -1 ){
		return ERR_INIT;
	}
	return pfnClearSendAddrDLL(m_iID);
}

//�f�[�^���M���J�n
//�߂�l�F�G���[�R�[�h
DWORD CSendTSTCPDllUtil::StartSend(
	)
{
	if( m_hModule == NULL || m_iID == -1 ){
		return ERR_INIT;
	}
	return pfnStartSendDLL(m_iID);
}

//�f�[�^���M���~
//�߂�l�F�G���[�R�[�h
DWORD CSendTSTCPDllUtil::StopSend(
	)
{
	if( m_hModule == NULL || m_iID == -1 ){
		return ERR_INIT;
	}
	return pfnStopSendDLL(m_iID);
}

//�f�[�^���M���J�n
//�߂�l�F�G���[�R�[�h
DWORD CSendTSTCPDllUtil::AddSendData(
	BYTE* pbData,
	DWORD dwSize
	)
{
	if( m_hModule == NULL || m_iID == -1 ){
		return ERR_INIT;
	}
	return pfnAddSendDataDLL(m_iID, pbData, dwSize);
}

//���M�o�b�t�@���N���A
//�߂�l�F�G���[�R�[�h
DWORD CSendTSTCPDllUtil::ClearSendBuff(
	)
{
	if( m_hModule == NULL || m_iID == -1 ){
		return ERR_INIT;
	}
	return pfnClearSendBuffDLL(m_iID);
}



