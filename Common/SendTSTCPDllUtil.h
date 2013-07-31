#ifndef __SEND_TS_TCP_DLL_UTIL_H__
#define __SEND_TS_TCP_DLL_UTIL_H__

#include "SendTSTCP.h"
#include "ErrDef.h"

using namespace sendtstcp;

class CSendTSTCPDllUtil
{
public:
	CSendTSTCPDllUtil(void);
	~CSendTSTCPDllUtil(void);

	//DLL�̏�����
	//�߂�l�F�G���[�R�[�h
	DWORD Initialize(
		);

	//DLL�̊J��
	//�߂�l�F�Ȃ�
	void UnInitialize(
		);

	//���M���ǉ�
	//�߂�l�F�G���[�R�[�h
	DWORD AddSendAddr(
		LPCWSTR lpcwszIP,
		DWORD dwPort
		);

	//���M��N���A
	//�߂�l�F�G���[�R�[�h
	DWORD ClearSendAddr(
		);

	//�f�[�^���M���J�n
	//�߂�l�F�G���[�R�[�h
	DWORD StartSend(
		);

	//�f�[�^���M���~
	//�߂�l�F�G���[�R�[�h
	DWORD StopSend(
		);

	//�f�[�^���M���J�n
	//�߂�l�F�G���[�R�[�h
	DWORD AddSendData(
		BYTE* pbData,
		DWORD dwSize
		);

	//���M�o�b�t�@���N���A
	//�߂�l�F�G���[�R�[�h
	DWORD ClearSendBuff(
		);
		
protected:
	HMODULE m_hModule;
	int m_iID;

	sendtstcp::InitializeDLL pfnInitializeDLL;
	sendtstcp::UnInitializeDLL pfnUnInitializeDLL;
	sendtstcp::AddSendAddrDLL pfnAddSendAddrDLL;
	sendtstcp::ClearSendAddrDLL pfnClearSendAddrDLL;
	sendtstcp::StartSendDLL pfnStartSendDLL;
	sendtstcp::StopSendDLL pfnStopSendDLL;
	sendtstcp::AddSendDataDLL pfnAddSendDataDLL;
	sendtstcp::ClearSendBuffDLL pfnClearSendBuffDLL;

protected:
	BOOL LoadDll(void);
	BOOL UnLoadDll(void);
};

#endif
