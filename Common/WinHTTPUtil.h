#pragma once

#include <winhttp.h>
#pragma comment (lib, "winhttp.lib")

#include "NWStructDef.h"
#include "StringUtil.h"
#include "ErrDef.h"

#define SEND_BUFF_SIZE 64*1024

class CWinHTTPUtil
{
public:
	CWinHTTPUtil(void);
	~CWinHTTPUtil(void);

	//�Z�b�V�������I�[�v������
	//�߂�l�F�G���[�R�[�h
	DWORD OpenSession(
		LPCWSTR agent,				//[IN] Agent�̐ݒ�
		BOOL async,					//[IN] �񓯊����[�h�œ������ǂ����iTRUE�F�񓯊��j
		BOOL useProxy,				//[IN] Proxy�g�����ǂ����iTRUE:Proxy�g���j
		USE_PROXY_INFO* proxyInfo	//[IN] Proxy�g���ꍇ�̐ݒ���
		);

	//�Z�b�V���������
	//�R�l�N�V�����ƃ��N�G�X�g������
	//�߂�l�F�G���[�R�[�h
	DWORD CloseSession();

	//�Z�b�V�������J����Ă��邩�ǂ���
	//�߂�l�F
	// TRUE�i�I�[�v���ς݁j�AFALSE�i���I�[�v���j
	BOOL IsOpenSession();

	//�Z�b�V�������񓯊��ŊJ����Ă��邩�ǂ���
	//�߂�l�F
	// TRUE�i�񓯊��j�AFALSE�i�����j
	BOOL IsOpenASync();

	//���N�G�X�g�𑗂�
	//�����������o�b�t�@��DL����ꍇ�̓t�@�C���e�ʂɗv����
	//�߂�l�F�G���[�R�[�h
	DWORD SendRequest( 
		LPCWSTR url,					//[IN] �A�N�Z�X����URL
		NW_VERB_MODE verbMode,			//[IN] VERB�̎��
		LPCWSTR addHttpHeader,			//[IN] Http�w�b�_�ɒǉ�������̂���Ȃ�w��
		LPCWSTR saveFilePath,			//[IN] DL�t�@�C�����ANULL���͓����������o�b�t�@��DL
		UPLOAD_DATA_LIST* upList		//[IN] �T�[�o�[�ɑ��M����f�[�^(PUT or POST)
		);

	//���N�G�X�g�����
	//�߂�l�F�G���[�R�[�h
	DWORD CloseRequest();

	//�����������o�b�t�@��DL�����ꍇ�Ƀf�[�^�����o��
	//�߂�l�F�G���[�R�[�h
	DWORD GetDLBuff(
		BYTE* buff, //[OUT] DL�����f�[�^ NULL�w��ŃT�C�Y�擾�\
		DWORD* size //[IN/OUT] buff�̃T�C�Y
		);

protected:
	HINTERNET session;
	HINTERNET connect;
	HINTERNET request;

	BOOL useProxy;
	wstring proxyID;
	wstring proxyPWD;
	BOOL asyncMode;

	wstring lastHost;
	DWORD lastPort;
	wstring saveFilePath;

	vector<UPLOAD_DATA*> upDataList;
	DWORD totalUpSize;
	HANDLE upThread;
	HANDLE upStopEvent;
	HANDLE writeCompEvent;
	HANDLE responseCompEvent;
	DWORD errEndCode;

	wstring responseHTTPHeader;
	DWORD responseStatusCode;
	DWORD responseContentSize;

	typedef struct _DL_DATA{
		DWORD size;
		BYTE* data;
		_DL_DATA(void){
			size = 0;
			data = NULL;
		};
		~_DL_DATA(void){
			SAFE_DELETE_ARRAY(data);
		};
	} DL_DATA;
	vector<DL_DATA*> dlBuffList;

protected:
	static void CALLBACK StatusCallback (
		HINTERNET hInternet,
		DWORD_PTR dwContext,
		DWORD dwInternetStatus,
		LPVOID lpvStatusInformation,
		DWORD dwStatusInformationLength
		);
	static UINT WINAPI UploadThread(LPVOID pParam);

	void StatusSendRequestComplete();
	void StatusWriteComplete(DWORD size);
	void StatusHeadersAvailable();
	void StatusDataAvailable(DWORD size);

	void ClearUpList();
};

