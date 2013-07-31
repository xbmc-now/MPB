#ifndef __SEND_TS_TCP_H__
#define __SEND_TS_TCP_H__

#include "Util.h"
namespace sendtstcp{
//DLL�̏�����
//�߂�l�F����ID�i-1�ŃG���[�j
typedef int (WINAPI *InitializeDLL)(
	);

//DLL�̊J��
//�߂�l�F�G���[�R�[�h
typedef DWORD (WINAPI *UnInitializeDLL)(
	int iID //[IN] InitializeDLL�̖߂�l
	);

//���M���ǉ�
//�߂�l�F�G���[�R�[�h
typedef DWORD (WINAPI *AddSendAddrDLL)(
	int iID, //[IN] InitializeDLL�̖߂�l
	LPCWSTR lpcwszIP,
	DWORD dwPort
	);

//���M��N���A
//�߂�l�F�G���[�R�[�h
typedef DWORD (WINAPI *ClearSendAddrDLL)(
	int iID //[IN] InitializeDLL�̖߂�l
	);

//�f�[�^���M���J�n
//�߂�l�F�G���[�R�[�h
typedef DWORD (WINAPI *StartSendDLL)(
	int iID //[IN] InitializeDLL�̖߂�l
	);

//�f�[�^���M���~
//�߂�l�F�G���[�R�[�h
typedef DWORD (WINAPI *StopSendDLL)(
	int iID //[IN] InitializeDLL�̖߂�l
	);

//�f�[�^���M���J�n
//�߂�l�F�G���[�R�[�h
typedef DWORD (WINAPI *AddSendDataDLL)(
	int iID, //[IN] InitializeDLL�̖߂�l
	BYTE* pbData,
	DWORD dwSize
	);

//���M�o�b�t�@���N���A
//�߂�l�F�G���[�R�[�h
typedef DWORD (WINAPI *ClearSendBuffDLL)(
	int iID //[IN] InitializeDLL�̖߂�l
	);

}
#endif
