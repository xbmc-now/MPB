#include "StdAfx.h"
#include "ProxyUtil.h"


#include <winhttp.h>
#pragma comment (lib, "winhttp.lib")

CProxyUtil::CProxyUtil(void)
{
	this->agent = L"";
	this->ieConfig = NULL;
	this->autoDetectConfig = NULL;
	this->autoScriptConfig = NULL;
}


CProxyUtil::~CProxyUtil(void)
{
	SAFE_DELETE(this->ieConfig);
	SAFE_DELETE(this->autoDetectConfig);
	SAFE_DELETE(this->autoScriptConfig);
}

//IE��Proxy�ݒ���擾����
//�߂�l�FTRUE�i�֐������j�AFALSE�i�֐����s�j
BOOL CProxyUtil::GetIEProxyConfig(
	CURRENT_USER_IE_PROXY_CONFIG** proxyConfig //[OUT]IE��Proxy�ݒ�
	)
{
	if( proxyConfig == NULL ){
		return FALSE;
	}
	SAFE_DELETE(this->ieConfig);

	WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ieInfo;
	//IE�̐ݒ�擾
	if( WinHttpGetIEProxyConfigForCurrentUser(&ieInfo) == TRUE ){
		this->ieConfig = new CURRENT_USER_IE_PROXY_CONFIG;

		this->ieConfig->autoDetect = ieInfo.fAutoDetect;

		size_t len = 0;
		//�X�N���v�g�A�h���X
		if( ieInfo.lpszAutoConfigUrl != NULL ){
			len = wcslen(ieInfo.lpszAutoConfigUrl)+1;
			if( len >0 ){
				this->ieConfig->autoConfigUrl = new WCHAR[len];
				ZeroMemory( this->ieConfig->autoConfigUrl, sizeof(WCHAR)*len );
				wcscpy_s( this->ieConfig->autoConfigUrl, len, ieInfo.lpszAutoConfigUrl );
			}
			GlobalFree(ieInfo.lpszAutoConfigUrl);
		}
		//Proxy�A�h���X
		if( ieInfo.lpszProxy != NULL ){
			wstring proxyUrl = L"";
			proxyUrl = ieInfo.lpszProxy;

			if(proxyUrl.find(L"http=") != string::npos ){
				wstring buff;
				Separate(proxyUrl, L"http=", buff, proxyUrl);
				Separate(proxyUrl, L";", buff, proxyUrl);

				if(buff.size() > 0 ){
					len = buff.size()+1;
					this->ieConfig->proxy = new WCHAR[len];
					ZeroMemory( this->ieConfig->proxy, sizeof(WCHAR)*len );
					wcscpy_s(this->ieConfig->proxy, len, buff.c_str());
				}
			}else{
				if(proxyUrl.size() > 0 ){
					len = proxyUrl.size()+1;
					this->ieConfig->proxy = new WCHAR[len];
					ZeroMemory( this->ieConfig->proxy, sizeof(WCHAR)*len );
					wcscpy_s(this->ieConfig->proxy, len, proxyUrl.c_str());
				}
			}

			GlobalFree(ieInfo.lpszProxy);
		}
		//��O�ݒ�
		if( ieInfo.lpszProxyBypass != NULL ){
			len = wcslen(ieInfo.lpszProxyBypass)+1;
			DWORD dwLen = lstrlen(ieInfo.lpszProxyBypass)+1;
			if( len >0 ){
				this->ieConfig->proxyBypass = new WCHAR[len];
				ZeroMemory( this->ieConfig->proxyBypass, sizeof(WCHAR)*len );
				wcscpy_s( this->ieConfig->proxyBypass, len, ieInfo.lpszProxyBypass );
			}
			GlobalFree(ieInfo.lpszProxyBypass);
		}

		*proxyConfig = this->ieConfig;
		return TRUE;
	}else{
		return FALSE;
	}
}

//�����I�Ɍ��o��Proxy�̃A�h���X���擾����
//�߂�l�FTRUE�i�֐������j�AFALSE�i�֐����s�j
BOOL CProxyUtil::GetProxyAutoDetect(
	LPCWSTR accessURL, //[IN]Proxy�ݒ�擾�̂��߂Ɏ����ɐڑ�����URL
	PROXY_CONFIG** proxyConfig //[OUT]Proxy�̏��
	)
{
	if( accessURL == NULL || proxyConfig == NULL ){
		return FALSE;
	}
	SAFE_DELETE(this->autoDetectConfig);

	HINTERNET httpSession = NULL;
	WINHTTP_AUTOPROXY_OPTIONS autoProxyOptions;
	WINHTTP_PROXY_INFO proxyInfo;
	ZeroMemory( &autoProxyOptions, sizeof(autoProxyOptions) );
	ZeroMemory( &proxyInfo, sizeof(proxyInfo) );

	httpSession = WinHttpOpen( this->agent.c_str(),
		WINHTTP_ACCESS_TYPE_NO_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS,0 
		);
	if( httpSession == NULL ){
		return FALSE;
	}

	autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT;
	autoProxyOptions.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP
		 |WINHTTP_AUTO_DETECT_TYPE_DNS_A;
	autoProxyOptions.fAutoLogonIfChallenged = TRUE;
	//Proxy���擾
	if( WinHttpGetProxyForUrl( httpSession, accessURL, &autoProxyOptions, &proxyInfo) == FALSE ){
		DWORD dwErr = GetLastError();
		WinHttpCloseHandle( httpSession );
		return FALSE;
	}
	this->autoDetectConfig = new PROXY_CONFIG;

	if (proxyInfo.lpszProxy != NULL){
		size_t len = wcslen(proxyInfo.lpszProxy)+1;
		if( len > 0 ){
			this->autoDetectConfig->proxy = new WCHAR[len];
			ZeroMemory( this->autoDetectConfig->proxy, sizeof(WCHAR)*len );
			wcscpy_s( this->autoDetectConfig->proxy, len, proxyInfo.lpszProxy );
		}
		GlobalFree(proxyInfo.lpszProxy);
	}
	if (proxyInfo.lpszProxyBypass != NULL){
		size_t len = wcslen(proxyInfo.lpszProxyBypass)+1;
		if( len > 0 ){
			this->autoDetectConfig->proxyBypass = new WCHAR[len];
			ZeroMemory( this->autoDetectConfig->proxyBypass, sizeof(WCHAR)*len );
			wcscpy_s( this->autoDetectConfig->proxyBypass, len, proxyInfo.lpszProxyBypass );
		}
		GlobalFree(proxyInfo.lpszProxyBypass);

	}

	*proxyConfig = this->autoDetectConfig;

	WinHttpCloseHandle( httpSession );
	return TRUE;
}

//�����\���X�N���v�g��Proxy�̃A�h���X���擾����
//�߂�l�FTRUE�i�֐������j�AFALSE�i�֐����s�j
BOOL CProxyUtil::GetProxyAutoScript(
	LPCWSTR accessURL, //[IN]Proxy�ݒ�擾�̂��߂Ɏ����ɐڑ�����URL
	LPCWSTR scriptURL, //[IN]�����\���X�N���v�g��URL
	PROXY_CONFIG** proxyConfig //[OUT]Proxy�̏��
	)
{
	if( accessURL == NULL || scriptURL == NULL || proxyConfig == NULL ){
		return FALSE;
	}
	SAFE_DELETE(this->autoScriptConfig);

	HINTERNET httpSession = NULL;
	WINHTTP_AUTOPROXY_OPTIONS autoProxyOptions;
	WINHTTP_PROXY_INFO proxyInfo;
	ZeroMemory( &autoProxyOptions, sizeof(autoProxyOptions) );
	ZeroMemory( &proxyInfo, sizeof(proxyInfo) );

	httpSession = WinHttpOpen( this->agent.c_str(),
		WINHTTP_ACCESS_TYPE_NO_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS,0 
		);
	if( httpSession == NULL ){
		return FALSE;
	}

	autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_CONFIG_URL;
	autoProxyOptions.lpszAutoConfigUrl = scriptURL;
	autoProxyOptions.fAutoLogonIfChallenged = TRUE;
	//Proxy���擾
	if( WinHttpGetProxyForUrl( httpSession, accessURL, &autoProxyOptions, &proxyInfo ) == FALSE ){
		WinHttpCloseHandle( httpSession );
		return FALSE;
	}

	this->autoScriptConfig = new PROXY_CONFIG;

	if (proxyInfo.lpszProxy != NULL){
		size_t len = wcslen(proxyInfo.lpszProxy)+1;
		if( len > 0 ){
			this->autoScriptConfig->proxy = new WCHAR[len];
			ZeroMemory( this->autoScriptConfig->proxy, sizeof(WCHAR)*len );
			wcscpy_s( this->autoScriptConfig->proxy, len, proxyInfo.lpszProxy );
		}
		GlobalFree(proxyInfo.lpszProxy);
	}
	if (proxyInfo.lpszProxyBypass != NULL){
		size_t len = wcslen(proxyInfo.lpszProxyBypass)+1;
		if( len > 0 ){
			this->autoScriptConfig->proxyBypass = new WCHAR[len];
			ZeroMemory( this->autoScriptConfig->proxyBypass, sizeof(WCHAR)*len );
			wcscpy_s( this->autoScriptConfig->proxyBypass, len, proxyInfo.lpszProxyBypass );
		}
		GlobalFree(proxyInfo.lpszProxyBypass);
	}

	*proxyConfig = this->autoScriptConfig;

	WinHttpCloseHandle( httpSession );
	return TRUE;
}
