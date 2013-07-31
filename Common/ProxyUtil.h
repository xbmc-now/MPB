#pragma once
#include "NWStructDef.h"
#include "StringUtil.h"

class CProxyUtil
{
public:
	CProxyUtil(void);
	~CProxyUtil(void);

	//IE��Proxy�ݒ���擾����
	//�߂�l�F
	// TRUE�i�֐������j�AFALSE�i�֐����s�j
	BOOL GetIEProxyConfig(
		CURRENT_USER_IE_PROXY_CONFIG** proxyConfig //[OUT]IE��Proxy�ݒ�
		);

	//�����I�Ɍ��o��Proxy�̃A�h���X���擾����
	//�߂�l�F
	// TRUE�i�֐������j�AFALSE�i�֐����s�j
	BOOL GetProxyAutoDetect(
		LPCWSTR accessURL, //[IN]Proxy�ݒ�擾�̂��߂Ɏ����ɐڑ�����URL
		PROXY_CONFIG** proxyConfig //[OUT]Proxy�̏��
		);

	//�����\���X�N���v�g��Proxy�̃A�h���X���擾����
	//�߂�l�F
	// TRUE�i�֐������j�AFALSE�i�֐����s�j
	BOOL GetProxyAutoScript(
		LPCWSTR accessURL, //[IN]Proxy�ݒ�擾�̂��߂Ɏ����ɐڑ�����URL
		LPCWSTR scriptURL, //[IN]�����\���X�N���v�g��URL
		PROXY_CONFIG** proxyConfig //[OUT]Proxy�̏��
		);

private:
	wstring agent;
	CURRENT_USER_IE_PROXY_CONFIG* ieConfig;
	PROXY_CONFIG* autoDetectConfig;
	PROXY_CONFIG* autoScriptConfig;
};

