#pragma once

#include <Windows.h>
#include "NWStructDef.h"


//DLL�̏�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// id				[OUT]����ID
typedef DWORD (WINAPI *InitializeTW)(
	DWORD* id
	);

//DLL�̊J��
//�߂�l�F
// �G���[�R�[�h
//�����F
// id		[IN]����ID
typedef DWORD (WINAPI *UnInitializeTW)(
	DWORD id
	);

//IE��Proxy�ݒ���擾����
//�߂�l�F
// TRUE�i�֐������j�AFALSE�i�֐����s�j
//�����F
// proxyConfig		[OUT]IE��Proxy�ݒ�̏��i����API�ďo���܂Ń������m�ہj
typedef BOOL (WINAPI *GetIEProxyConfigTW)(
	DWORD id,
	CURRENT_USER_IE_PROXY_CONFIG** proxyConfig
	);

//�����I�Ɍ��o��Proxy�̃A�h���X���擾����
//�߂�l�F
// TRUE�i�֐������j�AFALSE�i�֐����s�j
//�����F
// proxyConfig		[OUT]Proxy�̏��i����API�ďo���܂Ń������m�ہj
typedef BOOL (WINAPI *GetProxyAutoDetectTW)(
	DWORD id,
	PROXY_CONFIG** proxyConfig
	);

//�����\���X�N���v�g��Proxy�̃A�h���X���擾����
//�߂�l�F
// TRUE�i�֐������j�AFALSE�i�֐����s�j
//�����F
// scriptURL		[IN]�����\���X�N���v�g��URL
// proxyConfig		[OUT]Proxy�̏��i����API�ďo���܂Ń������m�ہj
typedef BOOL (WINAPI *GetProxyAutoScriptTW)(
	DWORD id,
	LPCWSTR scriptURL,
	PROXY_CONFIG** proxyConfig
	);

//Proxy�g�p��ݒ�
//�߂�l�F
// �G���[�R�[�h
//�����F
// useProxy			[IN]Proxy�g�����ǂ����iTRUE:Proxy�g���j
// proxyInfo		[IN]Proxy�g���ꍇ�̐ݒ���
typedef void (WINAPI *SetProxyTW)(
	DWORD id,
	BOOL useProxy,
	USE_PROXY_INFO* proxyInfo
	);

//�F�ؗp���O�C��URL���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// url			[OUT]�F�ؗp���O�C��URL�i����API�ďo���܂Ń������m�ہj
typedef DWORD (WINAPI *GetAuthorizationUrlTW)(
	DWORD id,
	WCHAR** url
	);

//�F�،��ʂ̈Ïؔԍ���ݒ肷��
//������GetAuthorizationUrl�Ŏ擾�����l���g�p����̂ŁA��A�̗���Őݒ肷��K�v����
//�߂�l�F
// �G���[�R�[�h
//�����F
// password		[IN]�Ïؔԍ�
typedef DWORD (WINAPI *SetAuthorizationPWDTW)(
	DWORD id,
	LPCWSTR password
	);

//�c�C�[�g����
//�߂�l�F
// �G���[�R�[�h
//�����F
// asyncMode	[IN]�񓯊��ő��M
// text			[IN]�c�C�[�g���e
typedef DWORD (WINAPI *SendTweetTW)(
	DWORD id,
	BOOL asyncMode,
	LPCWSTR text
	);

//�񓯊��c�C�[�g�̎c����擾����
//�߂�l�F
// ��
//�����F
typedef DWORD (WINAPI *GetTweetQueTW)(
	DWORD id
	);

class CTwitterUtil
{
public:
	CTwitterUtil(void);
	~CTwitterUtil(void);

	//DLL�̏�����
	//�߂�l�F
	// �G���[�R�[�h
	DWORD Initialize(
		);

	//DLL�̊J��
	//�߂�l�F
	// �G���[�R�[�h
	DWORD UnInitialize(
		);

	//IE��Proxy�ݒ���擾����
	//�߂�l�F
	// TRUE�i�֐������j�AFALSE�i�֐����s�j
	//�����F
	// proxyConfig		[OUT]IE��Proxy�ݒ�̏��i����API�ďo���܂Ń������m�ہj
	BOOL GetIEProxyConfig(
		CURRENT_USER_IE_PROXY_CONFIG** proxyConfig
		);

	//�����I�Ɍ��o��Proxy�̃A�h���X���擾����
	//�߂�l�F
	// TRUE�i�֐������j�AFALSE�i�֐����s�j
	//�����F
	// proxyConfig		[OUT]Proxy�̏��i����API�ďo���܂Ń������m�ہj
	BOOL GetProxyAutoDetect(
		PROXY_CONFIG** proxyConfig
		);

	//�����\���X�N���v�g��Proxy�̃A�h���X���擾����
	//�߂�l�F
	// TRUE�i�֐������j�AFALSE�i�֐����s�j
	//�����F
	// scriptURL		[IN]�����\���X�N���v�g��URL
	// proxyConfig		[OUT]Proxy�̏��i����API�ďo���܂Ń������m�ہj
	BOOL GetProxyAutoScript(
		LPCWSTR scriptURL,
		PROXY_CONFIG** proxyConfig
		);

	//Proxy�g�p��ݒ�
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// useProxy			[IN]Proxy�g�����ǂ����iTRUE:Proxy�g���j
	// proxyInfo		[IN]Proxy�g���ꍇ�̐ݒ���
	void SetProxy(
		BOOL useProxy,
		USE_PROXY_INFO* proxyInfo
		);

	//�F�ؗp���O�C��URL���擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// url			[OUT]�F�ؗp���O�C��URL�i����API�ďo���܂Ń������m�ہj
	DWORD GetAuthorizationUrl(
		WCHAR** url
		);

	//�F�،��ʂ̈Ïؔԍ���ݒ肷��
	//������GetAuthorizationUrl�Ŏ擾�����l���g�p����̂ŁA��A�̗���Őݒ肷��K�v����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// password		[IN]�Ïؔԍ�
	DWORD SetAuthorizationPWD(
		LPCWSTR password
		);

	//�c�C�[�g����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// asyncMode	[IN]�񓯊��ő��M
	// text			[IN]�c�C�[�g���e
	DWORD SendTweet(
		BOOL asyncMode,
		LPCWSTR text
		);

	//�񓯊��c�C�[�g�̎c����擾����
	//�߂�l�F
	// ��
	DWORD GetTweetQue(
		);

protected:
	HMODULE module;
	HANDLE lockEvent;
	DWORD id;

	InitializeTW				pfnInitializeTW;
	UnInitializeTW				pfnUnInitializeTW;
	GetIEProxyConfigTW			pfnGetIEProxyConfigTW;
	GetProxyAutoDetectTW		pfnGetProxyAutoDetectTW;
	GetProxyAutoScriptTW		pfnGetProxyAutoScriptTW;
	SetProxyTW					pfnSetProxyTW;
	GetAuthorizationUrlTW		pfnGetAuthorizationUrlTW;
	SetAuthorizationPWDTW		pfnSetAuthorizationPWDTW;
	SendTweetTW					pfnSendTweetTW;
	GetTweetQueTW				pfnGetTweetQueTW;

protected:
	//PublicAPI�r������p
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 20*1000);
	void UnLock(LPCWSTR log = NULL);

};

