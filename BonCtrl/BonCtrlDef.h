#ifndef __BON_CTRL_DEF_H__
#define __BON_CTRL_DEF_H__

#include "../Common/Util.h"

#define MUTEX_UDP_PORT_NAME			TEXT("Global\\EpgDataCap_Bon_UDP_PORT_")
#define MUTEX_TCP_PORT_NAME			TEXT("Global\\EpgDataCap_Bon_TCP_PORT_")
#define CHSET_SAVE_EVENT_WAIT		 _T("Global\\EpgTimer_ChSet")


//BonDriver�̃t�@�C���p�X���
typedef struct _BON_DRIVER_INFO{
	wstring filePath;	//BonDriver.dll�̃t�@�C���p�X
	wstring fileName;	//BonDriver.dll�̃t�@�C�����i�g���q�܂ށj
	wstring fileTitle;	//BonDriver.dll�̃t�@�C�����i�g���q�܂܂Ȃ��j
	_BON_DRIVER_INFO(void){
		filePath = L"";
		fileName = L"";
		fileTitle = L"";
	}
	//=�I�y���[�^�[�̏���
	_BON_DRIVER_INFO & operator= (const _BON_DRIVER_INFO & o) {
		filePath = o.filePath;
		fileName = o.fileName;
		fileTitle = o.fileTitle;
		return *this;
	}
} BON_DRIVER_INFO;

//BonDriver�̕����`�����l�����
typedef struct _BON_CH_INFO{
	wstring chName;		//�`�����l����
	DWORD ch;			//CH
	_BON_CH_INFO(void){
		chName = L"";
		ch = 0;
	}
	//=�I�y���[�^�[�̏���
	_BON_CH_INFO & operator= (const _BON_CH_INFO & o) {
		chName = o.chName;
		ch = o.ch;
		return *this;
	}
}BON_CH_INFO;

//BonDriver�̃`���[�i�[��ԏ��
typedef struct _BON_SPACE_INFO{
	wstring spaceName;					//�`���[�i�[��Ԗ�
	DWORD space;						//Space
	map<DWORD, BON_CH_INFO> chMap;		//�`�����l�����X�g�i�L�[ CH�j
	_BON_SPACE_INFO(void){
		spaceName = L"";
		space = 0;
	}
	//=�I�y���[�^�[�̏���
	_BON_SPACE_INFO & operator= (const _BON_SPACE_INFO & o) {
		spaceName = o.spaceName;
		space = o.space;
		chMap = o.chMap;
		return *this;
	}
}BON_SPACE_INFO;

//��M�f�[�^�̃o�b�t�@�����O�p
typedef struct _TS_DATA{
	BYTE* data;				//TS�f�[�^
	DWORD size;				//data�̃T�C�Y
	_TS_DATA(void){
		data = NULL;
		size = 0;
	}
	~_TS_DATA(void){
		SAFE_DELETE_ARRAY(data);
	}
} TS_DATA;

//�l�b�g���[�N���M�p�ݒ�
typedef struct _NW_SEND_INFO{
	wstring ipString;
	DWORD ip;
	DWORD port;
	BOOL broadcastFlag;
}NW_SEND_INFO;

//�T�[�r�X���
typedef struct _TS_SERVICE_INFO{
	WORD ONID;
	WORD TSID;
	WORD SID;
	BYTE serviceType;
	BOOL partialFlag;
	wstring serviceName;
	wstring networkName;
	BYTE remoteControlKeyID;
}TS_SERVICE_INFO;

//EPG�擾�p�T�[�r�X���
typedef struct _EPGCAP_SERVICE_INFO{
	WORD ONID;
	WORD TSID;
	WORD SID;
}EPGCAP_SERVICE_INFO;


#endif
