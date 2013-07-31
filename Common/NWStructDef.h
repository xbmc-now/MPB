#ifndef __NW_STRUCT_DEF_H__
#define __NW_STRUCT_DEF_H__

#include "Util.h"

enum NW_VERB_MODE{
	NW_VERB_GET = 0,
	NW_VERB_POST,
	NW_VERB_PUT,
};

//�ڑ����Ɏg�p����Proxy�̏��
typedef struct _USE_PROXY_INFO{
	WCHAR* serverName;		//�T�[�o�[���i�A�h���X:�|�[�g�ԍ��j
	WCHAR* proxyBypass;		//Proxy�̗�O�ݒ�
	WCHAR* userName;		//�F��Proxy���[�U�[��
	WCHAR* password;		//�F��Proxy�p�X���[�h
	_USE_PROXY_INFO(void){
		serverName = NULL;
		proxyBypass = NULL;
		userName = NULL;
		password = NULL;
	};
	~_USE_PROXY_INFO(void){
		SAFE_DELETE_ARRAY(serverName);
		SAFE_DELETE_ARRAY(proxyBypass);
		SAFE_DELETE_ARRAY(userName);
		SAFE_DELETE_ARRAY(password);
	};
	_USE_PROXY_INFO & operator= (const _USE_PROXY_INFO & o) {
		SAFE_DELETE_ARRAY(serverName);
		SAFE_DELETE_ARRAY(proxyBypass);
		SAFE_DELETE_ARRAY(userName);
		SAFE_DELETE_ARRAY(password);
		size_t len = 0;
		if( o.serverName != NULL ){
			len = wcslen(o.serverName);
			if( len > 0 ){
				serverName = new WCHAR[len+1];
				wcscpy_s(serverName, len+1, o.serverName);
			}
		}
		if( o.proxyBypass != NULL ){
			len = wcslen(o.proxyBypass);
			if( len > 0 ){
				proxyBypass = new WCHAR[len+1];
				wcscpy_s(proxyBypass, len+1, o.proxyBypass);
			}
		}
		if( o.userName != NULL ){
			len = wcslen(o.userName);
			if( len > 0 ){
				userName = new WCHAR[len+1];
				wcscpy_s(userName, len+1, o.userName);
			}
		}
		if( o.password != NULL ){
			len = wcslen(o.password);
			if( len > 0 ){
				password = new WCHAR[len+1];
				wcscpy_s(password, len+1, o.password);
			}
		}
		return *this;
	};
} USE_PROXY_INFO;

//IE��Proxy�̏��
typedef struct _CURRENT_USER_IE_PROXY_CONFIG{
	BOOL autoDetect;		//�ݒ�������I�Ɍ��o���邩�ǂ���
	WCHAR* autoConfigUrl;	//�����\���X�N���v�g��URL
	WCHAR* proxy;			//�T�[�o�[���i�A�h���X:�|�[�g�ԍ��j
	WCHAR* proxyBypass;		//Proxy�̗�O�ݒ�
	_CURRENT_USER_IE_PROXY_CONFIG(void){
		autoConfigUrl = NULL;
		proxy = NULL;
		proxyBypass = NULL;
	};
	~_CURRENT_USER_IE_PROXY_CONFIG(void){
		SAFE_DELETE_ARRAY(autoConfigUrl);
		SAFE_DELETE_ARRAY(proxy);
		SAFE_DELETE_ARRAY(proxyBypass);
	};
} CURRENT_USER_IE_PROXY_CONFIG;

//Proxy�̏��
typedef struct _PROXY_CONFIG{
	WCHAR* proxy;			//�T�[�o�[���i�A�h���X:�|�[�g�ԍ��j
	WCHAR* proxyBypass;		//Proxy�̗�O�ݒ�
	_PROXY_CONFIG(void){
		proxy = NULL;
		proxyBypass = NULL;
	};
	~_PROXY_CONFIG(void){
		SAFE_DELETE_ARRAY(proxy);
		SAFE_DELETE_ARRAY(proxyBypass);
	};
} PROXY_CONFIG;

//�A�b�v���[�h����f�[�^
typedef struct _UPLOAD_DATA{
	BOOL filePathFlag;		//buff���t�@�C���p�X���ǂ���
	DWORD buffSize;			//buff�̃T�C�Y�i�o�C�g�P�ʁj
	BYTE* buff;				//�f�[�^�̃o�b�t�@�i�t�@�C���p�X����WCHAR��\0�ŏI���o�b�t�@�j
	_UPLOAD_DATA(void){
		filePathFlag = FALSE;
		buffSize = 0;
		buff = NULL;
	};
	~_UPLOAD_DATA(void){
		SAFE_DELETE_ARRAY(buff);
	};
} UPLOAD_DATA;

//�A�b�v���[�h����f�[�^�̃��X�g
//�z��̏��ɃA�b�v���[�h�f�[�^���\�z����i�P���Ƀo�C�i���I�Ɍ����j
typedef struct _UPLOAD_DATA_LIST{
	DWORD listCount;		//list�̌�
	UPLOAD_DATA* list;		//UPLOAD_DATA�̔z��
	_UPLOAD_DATA_LIST(void){
		listCount = 0;
		list = NULL;
	};
	~_UPLOAD_DATA_LIST(void){
		SAFE_DELETE_ARRAY(list);
	};
} UPLOAD_DATA_LIST;

#endif
