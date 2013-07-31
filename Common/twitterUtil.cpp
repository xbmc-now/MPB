#include "StdAfx.h"
#include "twitterUtil.h"

#include "Util.h"
#include "ErrDef.h"

CTwitterUtil::CTwitterUtil(void)
{
	module = NULL;

	this->id = 0;
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL );
}

CTwitterUtil::~CTwitterUtil(void)
{
	UnInitialize();

	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
}

BOOL CTwitterUtil::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
	DWORD dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED){
		return FALSE;
	}
	return TRUE;
}

void CTwitterUtil::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

//DLL�̏�����
//�߂�l�F
// �G���[�R�[�h
DWORD CTwitterUtil::Initialize()
{
	if( module != NULL ){
		return FALSE;
	}
	if( Lock() == FALSE ) return FALSE;

	pfnInitializeTW = NULL;
	pfnUnInitializeTW = NULL;
	pfnGetIEProxyConfigTW = NULL;
	pfnGetProxyAutoDetectTW = NULL;
	pfnGetProxyAutoScriptTW = NULL;
	pfnSetProxyTW = NULL;
	pfnGetAuthorizationUrlTW = NULL;
	pfnSetAuthorizationPWDTW = NULL;
	pfnSendTweetTW = NULL;
	pfnGetTweetQueTW = NULL;


	BOOL ret = TRUE;

	module = ::LoadLibrary(L"twitter.dll");

	if( module == NULL ){
		UnLock();
		return FALSE;
	}

	pfnInitializeTW = ( InitializeTW ) ::GetProcAddress( module , "InitializeTW");
	if( !pfnInitializeTW ){
		OutputDebugString(L"InitializeTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnUnInitializeTW = ( UnInitializeTW ) ::GetProcAddress( module , "UnInitializeTW");
	if( !pfnUnInitializeTW ){
		OutputDebugString(L"UnInitializeTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetIEProxyConfigTW = ( GetIEProxyConfigTW ) ::GetProcAddress( module , "GetIEProxyConfigTW");
	if( !pfnGetIEProxyConfigTW ){
		OutputDebugString(L"GetIEProxyConfigTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetProxyAutoDetectTW = ( GetProxyAutoDetectTW ) ::GetProcAddress( module , "GetProxyAutoDetectTW");
	if( !pfnGetProxyAutoDetectTW ){
		OutputDebugString(L"GetProxyAutoDetectTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetProxyAutoScriptTW = ( GetProxyAutoScriptTW ) ::GetProcAddress( module , "GetProxyAutoScriptTW");
	if( !pfnGetProxyAutoScriptTW ){
		OutputDebugString(L"GetProxyAutoScriptTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnSetProxyTW = ( SetProxyTW ) ::GetProcAddress( module , "SetProxyTW");
	if( !pfnSetProxyTW ){
		OutputDebugString(L"SetProxyTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnGetAuthorizationUrlTW = ( GetAuthorizationUrlTW ) ::GetProcAddress( module , "GetAuthorizationUrlTW");
	if( !pfnGetAuthorizationUrlTW ){
		OutputDebugString(L"GetAuthorizationUrlTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}
	pfnSetAuthorizationPWDTW = ( SetAuthorizationPWDTW ) ::GetProcAddress( module , "SetAuthorizationPWDTW");
	if( !pfnSetAuthorizationPWDTW ){
		OutputDebugString(L"SetAuthorizationPWDTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}

	pfnSendTweetTW = ( SendTweetTW ) ::GetProcAddress( module , "SendTweetTW");
	if( !pfnSendTweetTW ){
		OutputDebugString(L"SendTweetTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}

	pfnGetTweetQueTW = ( GetTweetQueTW ) ::GetProcAddress( module , "GetTweetQueTW");
	if( !pfnGetTweetQueTW ){
		OutputDebugString(L"GetTweetQueTW�� GetProcAddress �Ɏ��s\r\n");
		ret = FALSE;
		goto ERR_END;
	}

	if( pfnInitializeTW(&this->id) == FALSE ){
		this->id = 0;
		ret = FALSE;
		goto ERR_END;
	}

ERR_END:
	if( ret == FALSE ){
		::FreeLibrary( module );
		module=NULL;
	}
	UnLock();
	return ret;
}

//DLL�̊J��
//�߂�l�F
// �G���[�R�[�h
DWORD CTwitterUtil::UnInitialize()
{
	if( module != NULL ){
		pfnUnInitializeTW(this->id);
		this->id = 0;
		::FreeLibrary( module );
	}
	module = NULL;
	
	pfnInitializeTW = NULL;
	pfnUnInitializeTW = NULL;
	pfnGetIEProxyConfigTW = NULL;
	pfnGetProxyAutoDetectTW = NULL;
	pfnGetProxyAutoScriptTW = NULL;
	pfnSetProxyTW = NULL;
	pfnGetAuthorizationUrlTW = NULL;
	pfnSetAuthorizationPWDTW = NULL;
	pfnSendTweetTW = NULL;
	pfnGetTweetQueTW = NULL;

	return NO_ERR;
}

//IE��Proxy�ݒ���擾����
//�߂�l�F
// TRUE�i�֐������j�AFALSE�i�֐����s�j
//�����F
// proxyConfig		[OUT]IE��Proxy�ݒ�̏��i����API�ďo���܂Ń������m�ہj
BOOL CTwitterUtil::GetIEProxyConfig(
	CURRENT_USER_IE_PROXY_CONFIG** proxyConfig
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}

	return pfnGetIEProxyConfigTW(this->id, proxyConfig);
}

//�����I�Ɍ��o��Proxy�̃A�h���X���擾����
//�߂�l�F
// TRUE�i�֐������j�AFALSE�i�֐����s�j
//�����F
// proxyConfig		[OUT]Proxy�̏��i����API�ďo���܂Ń������m�ہj
BOOL CTwitterUtil::GetProxyAutoDetect(
	PROXY_CONFIG** proxyConfig
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}

	return pfnGetProxyAutoDetectTW(this->id, proxyConfig);
}

//�����\���X�N���v�g��Proxy�̃A�h���X���擾����
//�߂�l�F
// TRUE�i�֐������j�AFALSE�i�֐����s�j
//�����F
// scriptURL		[IN]�����\���X�N���v�g��URL
// proxyConfig		[OUT]Proxy�̏��i����API�ďo���܂Ń������m�ہj
BOOL CTwitterUtil::GetProxyAutoScript(
	LPCWSTR scriptURL,
	PROXY_CONFIG** proxyConfig
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}

	return pfnGetProxyAutoScriptTW(this->id, scriptURL, proxyConfig);
}

//Proxy�g�p��ݒ�
//�߂�l�F
// �G���[�R�[�h
//�����F
// useProxy			[IN]Proxy�g�����ǂ����iTRUE:Proxy�g���j
// proxyInfo		[IN]Proxy�g���ꍇ�̐ݒ���
void CTwitterUtil::SetProxy(
	BOOL useProxy,
	USE_PROXY_INFO* proxyInfo
	)
{
	if( module == NULL ){
		return ;
	}

	return pfnSetProxyTW(this->id, useProxy, proxyInfo);
}

//�F�ؗp���O�C��URL���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// url			[OUT]�F�ؗp���O�C��URL�i����API�ďo���܂Ń������m�ہj
DWORD CTwitterUtil::GetAuthorizationUrl(
	WCHAR** url
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}

	return pfnGetAuthorizationUrlTW(this->id, url);
}

//�F�،��ʂ̈Ïؔԍ���ݒ肷��
//������GetAuthorizationUrl�Ŏ擾�����l���g�p����̂ŁA��A�̗���Őݒ肷��K�v����
//�߂�l�F
// �G���[�R�[�h
//�����F
// password		[IN]�Ïؔԍ�
DWORD CTwitterUtil::SetAuthorizationPWD(
	LPCWSTR password
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}

	return pfnSetAuthorizationPWDTW(this->id, password);
}

//�c�C�[�g����
//�߂�l�F
// �G���[�R�[�h
//�����F
// asyncMode	[IN]�񓯊��ő��M
// text			[IN]�c�C�[�g���e
DWORD CTwitterUtil::SendTweet(
	BOOL asyncMode,
	LPCWSTR text
	)
{
	if( module == NULL ){
		return ERR_NOT_INIT;
	}

	return pfnSendTweetTW(this->id, asyncMode, text);
}

//�񓯊��c�C�[�g�̎c����擾����
//�߂�l�F
// ��
DWORD CTwitterUtil::GetTweetQue(
	)
{
	if( module == NULL ){
		return 0;
	}

	return pfnGetTweetQueTW(this->id);
}
